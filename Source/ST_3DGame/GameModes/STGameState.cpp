// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ST_3DGame/Character/STPlayerController.h"
#include "ST_3DGame/Item/STCoinItem.h"
#include "ST_3DGame/Item/STSpawnVolume.h"
#include "ST_3DGame/System/STGameInstance.h"
#include "ST_3DGame/Debuffs/DebuffType.h"

ASTGameState::ASTGameState()
{
	if (WaveSettings.Num() == 0)
	{
		WaveSettings.AddDefaulted();

		if (WaveSettings.IsValidIndex(0))
		{
			WaveSettings[0].TotalItemSpawnCount = 40;
			WaveSettings[0].WaveDuration = 30.f;
		}
	}
}

const FDebuffInfo* ASTGameState::GetDebuffInfo(EDebuffType Type) const
{
	if (!DebuffInfoTable) return nullptr;

	const FString EnumFullName  = UEnum::GetValueAsString(Type);

	// "::"를 기준으로 문자열을 분리하여, 마지막 부분만 가져옵니다.
	FString EnumValueName;
	EnumFullName.Split(TEXT("::"), nullptr, &EnumValueName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	
	const FName RowName = FName(*EnumValueName);

	return DebuffInfoTable->FindRow<FDebuffInfo>(RowName, TEXT(""));
}

void ASTGameState::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<USTGameInstance>(GetGameInstance());

	CurrentWaveIndex = 0;
	StartWave();
}

int32 ASTGameState::GetScore() const
{
	if (GameInstance)
	{
		return GameInstance->GetTotalScore();
	}
	return 0;
}

void ASTGameState::AddScore(int32 Amount)
{
	if (GameInstance)
	{
		GameInstance->AddToScore(Amount);

		OnScoreChanged.Broadcast(GetScore());
	}
}

int32 ASTGameState::GetCurrentWaveIndex() const
{
	return CurrentWaveIndex;
}

void ASTGameState::OnGameOver()
{
	if (ASTPlayerController* STPlayerController = Cast<ASTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		STPlayerController->SetPause(true);
		STPlayerController->ShowMainMenu(true);
	}
}

void ASTGameState::StartWave()
{
	if (!WaveSettings.IsValidIndex(CurrentWaveIndex))
	{
		OnGameOver();
		return;
	}

	OnWaveChanged.Broadcast(CurrentWaveIndex + 1);

	const FWaveData& CurrentWaveData = WaveSettings[CurrentWaveIndex];

	if (ASTPlayerController* STPlayerController = Cast<ASTPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		STPlayerController->ShowGameHUD();
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTSpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("레벨에 스폰 볼륨이 존재하지 않습니다. 아이템을 스폰할 수 없습니다."));
		return;
	}

	for (int32 i = 0; i < CurrentWaveData.TotalItemSpawnCount; ++i)
	{
		const int32 RandomVolumeIndex = FMath::RandRange(0, FoundVolumes.Num() - 1);
		if (ASTSpawnVolume* SpawnVolume = Cast<ASTSpawnVolume>(FoundVolumes[RandomVolumeIndex]))
		{
			AActor* SpawnActor = SpawnVolume->SpawnRandomItem();
			if (SpawnActor && SpawnActor->IsA(ASTCoinItem::StaticClass()))
			{
				SpawnedCoinCount++;
			}
		}
	}
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &ASTGameState::OnWaveTimeUp,
	                                CurrentWaveData.WaveDuration, false);
	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작 스폰된 코인: %d개"), CurrentWaveIndex + 1, SpawnedCoinCount);
}

void ASTGameState::OnWaveTimeUp()
{
	EndWave(true);
}

void ASTGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("코인 획득: %d / %d"), CollectedCoinCount, SpawnedCoinCount);
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave(false);
	}
}

void ASTGameState::EndWave(bool bIsTimeUp)
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	if (bIsTimeUp)
	{
		UE_LOG(LogTemp, Warning, TEXT("시간 초과 Wave %d 실패."), CurrentWaveIndex + 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave %d 성공!"), CurrentWaveIndex + 1);
	}

	CurrentWaveIndex++;
	StartWave();
}

float ASTGameState::GetWaveRemainingTime() const
{
	return GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ST_3DGame/Character/STCharacter.h"
#include "ST_3DGame/Character/STPlayerController.h"
#include "ST_3DGame/Item/STCoinItem.h"
#include "ST_3DGame/Item/STSpawnVolume.h"
#include "ST_3DGame/System/STGameInstance.h"

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

void ASTGameState::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<USTGameInstance>(GetGameInstance());
	UpdateHUD();

	CurrentWaveIndex = 0;
	StartWave();

	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ASTGameState::UpdateHUD, 0.1f, true);
}

int32 ASTGameState::GetScore() const
{
	return Score;
}

void ASTGameState::AddScore(int32 Amount)
{
	if (GameInstance)
	{
		GameInstance->AddToScore(Amount);
	}
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
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작 스폰된 코인: %d개"), CurrentWaveIndex + 1, SpawnedCoinCount);
}

void ASTGameState::OnWaveTimeUp()
{
	EndWave(true);
}

void ASTGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UpdateHUD();
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

void ASTGameState::UpdateHUD()
{
	ASTPlayerController* STPlayerController = Cast<ASTPlayerController>(GetWorld()->GetFirstPlayerController());
	ensure(STPlayerController);

	UUserWidget* HUDWidget = STPlayerController->GetHUDWidget();
	if (!HUDWidget)
	{
		return;
	}

	if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
		if (RemainingTime == -1.0f)
		{
			TimeText->SetText(FText::FromString(FString::Printf(TEXT("종료"))));
		}
		else
		{
			TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
		}
	}

	if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
	{
		if (GameInstance)
		{
			ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), GameInstance->GetTotalScore())));
		}
	}

	if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
	{
		if (WaveSettings.IsValidIndex(CurrentWaveIndex))
		{
			WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
		}
		else
		{
			WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT(""))));
		}
	}

	if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
	{
		if (ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(STPlayerController->GetPawn()))
		{
			HealthBar->SetPercent(static_cast<float>(PlayerCharacter->GetHealth()) / PlayerCharacter->GetMaxHealth());
		}
	}
}

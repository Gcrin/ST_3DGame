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

ASTGameState::ASTGameState(): Score(0), TotalCoinSpawnCount(40), SpawnedCoinCount(0), CollectedCoinCount(0),
                              LevelDuration(30.0f),
                              CurrentLevelIndex(0), MaxLevels(3)
{
}

void ASTGameState::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<USTGameInstance>(GetGameInstance());
	UpdateHUD();
	StartLevel();

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
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}

void ASTGameState::StartLevel()
{
	if (GameInstance)
	{
		CurrentLevelIndex = GameInstance->GetCurrentLevelIndex();
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

	for (int32 i = 0; i < TotalCoinSpawnCount; ++i)
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
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &ASTGameState::OnLevelTimeUp, LevelDuration, false);
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("레벨 %d 시작! 스폰된 코인: %d개"), CurrentLevelIndex + 1, SpawnedCoinCount);
}

void ASTGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASTGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("코인 획득: %d / %d"), CollectedCoinCount, SpawnedCoinCount);
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ASTGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (GameInstance)
	{
		AddScore(Score);
		CurrentLevelIndex++;
		GameInstance->SetCurrentLevelIndex(CurrentLevelIndex);
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASTGameState::UpdateHUD()
{
	ASTPlayerController* STPlayerController = Cast<ASTPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!STPlayerController)
	{
		return;
	}

	UUserWidget* HUDWidget = STPlayerController->GetHUDWidget();
	if (!HUDWidget)
	{
		return;
	}

	if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
	{
		float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
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

	if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
	{
		if (CurrentLevelIndex >= MaxLevels)
		{
			LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT(""))));
		}
		else
		{
			LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
		}
	}

	if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
	{
		if (ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(STPlayerController->GetPawn()))
		{
			HealthBar->SetPercent(PlayerCharacter->GetHealth() / PlayerCharacter->GetMaxHealth());
		}
	}
}

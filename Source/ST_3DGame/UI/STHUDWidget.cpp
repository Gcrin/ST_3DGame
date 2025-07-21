// Fill out your copyright notice in the Description page of Project Settings.


#include "STHUDWidget.h"

#include "STDebuffIconWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "ST_3DGame/GameModes/STGameState.h"
#include "ST_3DGame/Character/STCharacter.h"
#include "TimerManager.h"
#include "Components/NamedSlot.h"

void USTHUDWidget::AddGameScreenEffect(UUserWidget* EffectWidget)
{
	check(GameScreenEffectsSlot != nullptr);
	
	if (EffectWidget)
	{
		GameScreenEffectsSlot->AddChild(EffectWidget);
	}
}

void USTHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ASTGameState* STGameState = GetWorld()->GetGameState<ASTGameState>())
	{
		STGameState->OnScoreChanged.AddDynamic(this, &USTHUDWidget::UpdateScore);
		STGameState->OnWaveChanged.AddDynamic(this, &USTHUDWidget::UpdateWave);

		UpdateScore(STGameState->GetScore());
		UpdateWave(STGameState->GetCurrentWaveIndex() + 1);
	}
	if (APawn* PlayerPawn = GetOwningPlayerPawn())
	{
		if (ASTCharacter* STCharacter = Cast<ASTCharacter>(PlayerPawn))
		{
			STCharacter->OnHealthChanged.AddDynamic(this, &USTHUDWidget::UpdateHealth);
			UpdateHealth(STCharacter->GetHealth(), STCharacter->GetMaxHealth());

			STCharacter->OnActiveDebuffsChanged.AddDynamic(this, &USTHUDWidget::UpdateDebuffIcons);
		}
	}
}

void USTHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TimeText)
	{
		if (ASTGameState* STGameState = GetWorld()->GetGameState<ASTGameState>())
		{
			const float RemainingTime = STGameState->GetWaveRemainingTime();

			if (RemainingTime < 0.f)
			{
				TimeText->SetText(FText::FromString(TEXT("종료")));
			}
			else
			{
				TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
			}
		}
	}
}

void USTHUDWidget::UpdateScore(int32 NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), NewScore)));
	}
}

void USTHUDWidget::UpdateWave(int32 NewWave)
{
	if (WaveText)
	{
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), NewWave)));
	}

	if (WaveAnnouncementWidgetClass)
	{
		if (UUserWidget* AnnouncementWidget = CreateWidget(this, WaveAnnouncementWidgetClass))
		{
			if (UTextBlock* AnnouncementTextBlock = Cast<UTextBlock>(
				AnnouncementWidget->GetWidgetFromName(TEXT("AnnouncementText"))))
			{
				AnnouncementTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Wave %d 시작"), NewWave)));
			}

			AnnouncementWidget->AddToViewport();
		}
	}
}

void USTHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar && MaxHealth > 0.0f)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
		if (HealthText)
		{
			HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth)));
		}
	}
}

void USTHUDWidget::UpdateDebuffIcons(const TArray<FActiveDebuff>& ActiveDebuffs)
{
	if (!DebuffBox || !DebuffIconWidgetClass) return;

	DebuffBox->ClearChildren();

	ASTGameState* STGameState = GetWorld()->GetGameState<ASTGameState>();
	if (!STGameState) return;

	for (const FActiveDebuff& Debuff : ActiveDebuffs)
	{
		if (const FDebuffInfo* StaticInfo = STGameState->GetDebuffInfo(Debuff.Type))
		{
			if (USTDebuffIconWidget* IconWidget = CreateWidget<USTDebuffIconWidget>(this, DebuffIconWidgetClass))
			{
				// 아이콘 위젯에 필요한 모든 정보를 전달
				IconWidget->InitializeIcon(Debuff, StaticInfo);
				DebuffBox->AddChildToHorizontalBox(IconWidget);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STHUDWidget.generated.h"

struct FActiveDebuff;
class UHorizontalBox;
class UImage;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ST_3DGAME_API USTHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddGameScreenEffect(UUserWidget* EffectWidget);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> DebuffBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNamedSlot> GameScreenEffectsSlot;

	UPROPERTY(EditAnywhere, Category = "Debuff")
	TSubclassOf<UUserWidget> DebuffIconWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Announcement")
	TSubclassOf<UUserWidget> WaveAnnouncementWidgetClass;

private:
	UFUNCTION()
	void UpdateScore(int32 NewScore);
	UFUNCTION()
	void UpdateWave(int32 NewWave);
	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void UpdateDebuffIcons(const TArray<FActiveDebuff>& ActiveDebuffs);
};

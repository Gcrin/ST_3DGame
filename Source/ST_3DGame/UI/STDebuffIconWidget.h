// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ST_3DGame/Character/STCharacter.h"
#include "STDebuffIconWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ST_3DGAME_API USTDebuffIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeIcon(const FActiveDebuff& DebuffInfo, const FDebuffInfo* StaticDebuffInfo);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

private:
	// 이 위젯이 표시하는 디버프의 타이머 핸들
	FTimerHandle DebuffTimerHandle;
};

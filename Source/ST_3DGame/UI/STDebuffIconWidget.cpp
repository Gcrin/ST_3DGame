// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffIconWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void USTDebuffIconWidget::InitializeIcon(const FActiveDebuff& DebuffInfo, const FDebuffInfo* StaticDebuffInfo)
{
	if (StaticDebuffInfo && IconImage)
	{
		// 데이터 테이블에서 읽어온 아이콘으로 이미지를 설정
		IconImage->SetBrushFromTexture(StaticDebuffInfo->Icon);
	}
	// 캐릭터로부터 받은 타이머 핸들을 복사
	DebuffTimerHandle = DebuffInfo.TimerHandle;
}

void USTDebuffIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TimeText && DebuffTimerHandle.IsValid())
	{
		// 타이머의 남은 시간을 가져와 텍스트로 표시
		const float RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(DebuffTimerHandle);
		if (RemainingTime > 0.f)
		{
			TimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RemainingTime)));
		}
	}
}

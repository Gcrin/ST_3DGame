// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffBlind.h"

#include "ST_3DGame/Character/STCharacter.h"
#include "Blueprint/UserWidget.h"
#include "ST_3DGame/Character/STPlayerController.h"
#include "ST_3DGame/UI/STHUDWidget.h"

void USTDebuffBlind::OnApply(ASTCharacter* Target)
{
	if (!Target || !Target->IsLocallyControlled() || !BlindEffectWidgetClass) return;

	if (ASTPlayerController* PlayerController = Target->GetController<ASTPlayerController>())
	{
		if (USTHUDWidget* HUD = Cast<USTHUDWidget>(PlayerController->GetHUDWidget()))
		{
			// 이전에 생성된 위젯이 혹시 남아있다면 제거.
			if (BlindEffectWidgetInstance)
			{
				BlindEffectWidgetInstance->RemoveFromParent();
			}

			// 블라인드 효과 위젯을 생성하고 뷰포트에 추가.
			BlindEffectWidgetInstance = CreateWidget(PlayerController, BlindEffectWidgetClass);
			if (BlindEffectWidgetInstance)
			{
				HUD->AddGameScreenEffect(BlindEffectWidgetInstance);
			}
		}
	}

	Target->SetIsBlinded(true);
}

void USTDebuffBlind::OnRemove(ASTCharacter* Target)
{
	if (BlindEffectWidgetInstance)
	{
		BlindEffectWidgetInstance->RemoveFromParent();
		BlindEffectWidgetInstance = nullptr;
	}

	if (Target)
	{
		Target->SetIsBlinded(false);
	}
}

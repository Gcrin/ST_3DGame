// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffReverseControl.h"

#include "ST_3DGame/Character/STCharacter.h"


void USTDebuffReverseControl::OnApply(ASTCharacter* Target)
{
	if (Target)
	{
		Target->SetIsControlsReversed(true);
	}
}

void USTDebuffReverseControl::OnRemove(ASTCharacter* Target)
{
	if (Target)
	{
		Target->SetIsControlsReversed(false);
	}
}

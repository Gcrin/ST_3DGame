// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffSlowing.h"

#include "ST_3DGame/Character/STCharacter.h"

void USTDebuffSlowing::OnApply(ASTCharacter* Target)
{
	if (Target)
	{
		Target->SetIsSlowed(true, 0.5f);
		Target->UpdateCharacterSpeed();
	}
}

void USTDebuffSlowing::OnRemove(ASTCharacter* Target)
{
	if (Target)
	{
		Target->SetIsSlowed(false, 1.0f);
		Target->UpdateCharacterSpeed();
	}
}

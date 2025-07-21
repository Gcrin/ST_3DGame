// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffItem.h"

#include "ST_3DGame/Character/STCharacter.h"

void ASTDebuffItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(Activator))
	{
		PlayerCharacter->ApplyDebuff(DebuffToApply, DebuffDuration);
	}

	DestroyItem();
}

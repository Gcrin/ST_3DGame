// Fill out your copyright notice in the Description page of Project Settings.


#include "STSlowingItem.h"

#include "ST_3DGame/Character/STCharacter.h"

void ASTSlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(Activator))
	{
		PlayerCharacter->ApplySlowingEffect(DebuffDuration, SpeedMultiplier);
	}

	DestroyItem();
}

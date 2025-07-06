// Fill out your copyright notice in the Description page of Project Settings.


#include "STHealingItem.h"
#include "ST_3DGame/Character/STCharacter.h"

ASTHealingItem::ASTHealingItem(): HealAmount(20)
{
	ItemType = "Healing";
}

void ASTHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}

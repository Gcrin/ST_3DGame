// Fill out your copyright notice in the Description page of Project Settings.


#include "STHealingItem.h"

ASTHealingItem::ASTHealingItem(): HealAmount(20)
{
	ItemType = "Healing";
}

void ASTHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("플레이어 %d만큼 HP 회복"), HealAmount));
        
		DestroyItem();
	}
}

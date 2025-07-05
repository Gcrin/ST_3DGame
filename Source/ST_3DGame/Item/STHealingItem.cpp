// Fill out your copyright notice in the Description page of Project Settings.


#include "STHealingItem.h"

ASTHealingItem::ASTHealingItem(): HealAmount(20)
{
	ItemType = "Healing";
}

void ASTHealingItem::ActivateItem(AActor* Activator)
{
	Destroy();
}

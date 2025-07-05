// Fill out your copyright notice in the Description page of Project Settings.


#include "STMineItem.h"

ASTMineItem::ASTMineItem(): ExplosionDelay(5.0f), ExplosionRadius(300.0f), ExplosionDamage(30.0f)
{
	ItemType = "Mine";
}

void ASTMineItem::ActivateItem(AActor* Activator)
{
	Destroy();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "STCoinItem.h"

ASTCoinItem::ASTCoinItem(): PointValue(0)
{
	ItemType = "DefaultCoin";
}

void ASTCoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
		                                 FString::Printf(TEXT("%d 포인트를 얻음"), PointValue));
		DestroyItem();
	}
}

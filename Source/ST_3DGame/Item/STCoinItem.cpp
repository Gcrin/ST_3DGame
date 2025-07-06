// Fill out your copyright notice in the Description page of Project Settings.


#include "STCoinItem.h"
#include "ST_3DGame/STGameState.h"

ASTCoinItem::ASTCoinItem(): PointValue(0)
{
	ItemType = "DefaultCoin";
}

void ASTCoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASTGameState* GameState = World->GetGameState<ASTGameState>())
			{
				GameState->AddScore(PointValue);
			}
		}
		DestroyItem();
	}
}

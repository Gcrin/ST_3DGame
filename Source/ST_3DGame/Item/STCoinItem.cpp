// Fill out your copyright notice in the Description page of Project Settings.


#include "STCoinItem.h"
#include "ST_3DGame/Character/STCharacter.h"
#include "ST_3DGame/GameModes/STGameState.h"

ASTCoinItem::ASTCoinItem(): PointValue(0)
{
	ItemType = "DefaultCoin";
}

void ASTCoinItem::ActivateItem(AActor* Activator)
{
	ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(Activator);
	if (!PlayerCharacter)
	{
		return;
	}
	
	ASTGameState* GameState = GetWorld()->GetGameState<ASTGameState>();
	if (!GameState)
	{
		return;
	}
	
	GameState->AddScore(PointValue);
	GameState->OnCoinCollected();
	
	DestroyItem();
}

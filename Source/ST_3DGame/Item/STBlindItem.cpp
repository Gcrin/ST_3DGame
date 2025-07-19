// Fill out your copyright notice in the Description page of Project Settings.


#include "STBlindItem.h"

#include "ST_3DGame/Character/STCharacter.h"

void ASTBlindItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (ASTCharacter* PlayerCharacter = Cast<ASTCharacter>(Activator))
	{
		PlayerCharacter->ApplyBlindEffect(DebuffDuration);
	}

	DestroyItem();
}

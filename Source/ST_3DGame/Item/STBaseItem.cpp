// Fill out your copyright notice in the Description page of Project Settings.


#include "STBaseItem.h"

// Sets default values
ASTBaseItem::ASTBaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASTBaseItem::OnItemOverlap(AActor* OverlapActor)
{
}

void ASTBaseItem::OnItemEndOverlap(AActor* OverlapActor)
{
}

void ASTBaseItem::ActivateItem(AActor* Activator)
{
}

FName ASTBaseItem::GetItemType() const
{
	return ItemType;
}

void ASTBaseItem::DestroyItem()
{
	Destroy();
}

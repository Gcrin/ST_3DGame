// Fill out your copyright notice in the Description page of Project Settings.


#include "STBigCoinItem.h"

ASTBigCoinItem::ASTBigCoinItem()
{
	PointValue = 100;
	ItemType = "BigCoin";
}

void ASTBigCoinItem::ActivateItem(AActor* Activator)
{
	Destroyed();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "STSmallCoinItem.h"

ASTSmallCoinItem::ASTSmallCoinItem()
{
	PointValue = 10;
	ItemType = "SmallCoin";
}

void ASTSmallCoinItem::ActivateItem(AActor* Activator)
{
	Destroyed();
}

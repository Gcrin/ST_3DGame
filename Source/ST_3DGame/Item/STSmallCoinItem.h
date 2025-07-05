// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STCoinItem.h"
#include "STSmallCoinItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTSmallCoinItem : public ASTCoinItem
{
	GENERATED_BODY()

public:
	ASTSmallCoinItem();
	virtual void ActivateItem(AActor* Activator) override;
};

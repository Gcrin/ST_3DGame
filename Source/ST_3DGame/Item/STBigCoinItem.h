// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STCoinItem.h"
#include "STBigCoinItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTBigCoinItem : public ASTCoinItem
{
	GENERATED_BODY()

public:
	ASTBigCoinItem();
	virtual void ActivateItem(AActor* Activator) override;
};

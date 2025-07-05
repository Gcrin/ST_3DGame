// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBaseItem.h"
#include "STCoinItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTCoinItem : public ASTBaseItem
{
	GENERATED_BODY()

public:
	ASTCoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
};

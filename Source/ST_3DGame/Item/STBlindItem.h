// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBaseItem.h"
#include "STBlindItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTBlindItem : public ASTBaseItem
{
	GENERATED_BODY()

protected:
	virtual void ActivateItem(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float DebuffDuration = 3.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBaseItem.h"
#include "STHealingItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTHealingItem : public ASTBaseItem
{
	GENERATED_BODY()

public:
	ASTHealingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Healing")
	float HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};

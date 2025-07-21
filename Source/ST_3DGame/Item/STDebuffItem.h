// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBaseItem.h"
#include "ST_3DGame/Debuffs/DebuffType.h"
#include "STDebuffItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTDebuffItem : public ASTBaseItem
{
	GENERATED_BODY()

protected:
	virtual void ActivateItem(AActor* Activator) override;

	//디버프 종류
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debuff")
	EDebuffType DebuffToApply = EDebuffType::None;

	//디버프 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debuff")
	float DebuffDuration = 4.0f;
};

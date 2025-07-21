// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STDebuffEffectBase.h"
#include "STDebuffPoison.generated.h"

class ASTCharacter;
/**
 * 
 */
UCLASS()
class ST_3DGAME_API USTDebuffPoison : public USTDebuffEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnTick(ASTCharacter* Target, float DeltaTime) override;

protected:
	// 초당 피해량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Poison Effect")
	float DamagePerSecond = 1.0f;

private:
	float TickAccumulator = 0.0f;
};

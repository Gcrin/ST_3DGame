// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STDebuffEffectBase.h"
#include "STDebuffBlind.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API USTDebuffBlind : public USTDebuffEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnApply(ASTCharacter* Target) override;
	virtual void OnRemove(ASTCharacter* Target) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blind Effect")
	TSubclassOf<UUserWidget> BlindEffectWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> BlindEffectWidgetInstance;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASTGameStateBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;

	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API USTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USTGameInstance();


	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetTotalScore(int32 NewTotalScore);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SetCurrentLevelIndex(int32 NewLevelIndex);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetTotalScore() const;
	UFUNCTION(BlueprintCallable, Category = "GameData")
	int32 GetCurrentLevelIndex() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;
};

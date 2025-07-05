// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "STItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct ST_3DGAME_API FSTItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};

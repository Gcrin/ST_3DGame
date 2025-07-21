#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DebuffType.generated.h"

class USTDebuffEffectBase;

UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	None			UMETA(DisplayName = "None"),
	Slowing			UMETA(DisplayName = "Slowing"),
	Blind			UMETA(DisplayName = "Blind"),
	ReverseControl	UMETA(DisplayName = "ReverseControl"),
	Poison			UMETA(DisplayName = "Poison")
};

USTRUCT(BlueprintType)
struct FDebuffInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USTDebuffEffectBase> EffectClass;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBaseItem.h"
#include "STMineItem.generated.h"

/**
 * 
 */
UCLASS()
class ST_3DGAME_API ASTMineItem : public ASTBaseItem
{
	GENERATED_BODY()

public:
	ASTMineItem();

	//폭발까지 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDelay;
	//폭발 범위 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionRadius;
	//폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDamage;

	virtual void ActivateItem(AActor* Activator) override;
};

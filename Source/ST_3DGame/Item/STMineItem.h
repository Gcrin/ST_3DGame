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
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	TObjectPtr<USphereComponent> ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<UParticleSystem> ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<USoundBase> ExplosionSound;

	//폭발까지 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDelay = 1.0f;
	//폭발 범위 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionRadius = 200.0f;
	//폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDamage = 30.0f;

	bool bHasExploded = false;
	FTimerHandle ExplosionTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;

	void Explode();
};

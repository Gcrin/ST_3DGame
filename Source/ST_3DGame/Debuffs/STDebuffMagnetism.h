// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STDebuffEffectBase.h"
#include "STDebuffMagnetism.generated.h"

class ASTBaseItem;
class USphereComponent;
/**
 * 
 */
UCLASS()
class ST_3DGAME_API USTDebuffMagnetism : public USTDebuffEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnApply(ASTCharacter* Target) override;
	virtual void OnTick(ASTCharacter* Target, float DeltaTime) override;
	virtual void OnRemove(ASTCharacter* Target) override;

protected:
	// 자성의 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magnetism Effect")
	float MagnetRadius = 700.0f;

	// 아이템이 끌려오는 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magnetism Effect")
	float AttractionSpeed = 4.0f;

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<ASTBaseItem>> AttractedItems;

	UFUNCTION()
	void OnFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                         const FHitResult& SweepResult);
	UFUNCTION()
	void OnFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STItemInterface.h"
#include "STBaseItem.generated.h"

class USphereComponent;

UCLASS()
class ST_3DGAME_API ASTBaseItem : public AActor, public ISTItemInterface
{
	GENERATED_BODY()

public:
	ASTBaseItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	TObjectPtr<USphereComponent> Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<UParticleSystem> PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	float ParticleDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	TObjectPtr<USoundBase> PickupSound;
    virtual void OnItemOverlap(
		    UPrimitiveComponent* OverlappedComp,
		    AActor* OtherActor,
		    UPrimitiveComponent* OtherComp,
		    int32 OtherBodyIndex,
		    bool bFromSweep,
		    const FHitResult& SweepResult) override;
    virtual void OnItemEndOverlap(
		    UPrimitiveComponent* OverlappedComp,
		    AActor* OtherActor,
		    UPrimitiveComponent* OtherComp,
		    int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	virtual void DestroyItem();
};

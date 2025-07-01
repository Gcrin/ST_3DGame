// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STItem.generated.h"

UCLASS()
class ST_3DGAME_API ASTItem : public AActor
{
	GENERATED_BODY()

public:
	ASTItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Properties")
	float RotationSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;
};

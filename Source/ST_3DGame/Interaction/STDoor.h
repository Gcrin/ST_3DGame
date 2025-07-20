// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STInteractionInterface.h"
#include "Components/TimelineComponent.h"
#include "STDoor.generated.h"

class UWidgetComponent;

UCLASS()
class ST_3DGAME_API ASTDoor : public AActor, public ISTInteractionInterface
{
	GENERATED_BODY()

public:
	ASTDoor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual void ShowInteractionPrompt_Implementation() override;
	virtual void HideInteractionPrompt_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> InteractionPromptWidget;

	UPROPERTY()
	FTimeline DoorTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	TObjectPtr<UCurveFloat> DoorRotationCurve;

	UFUNCTION()
	void UpdateDoorRotation(float Value);

private:
	bool bIsOpen = false;
};

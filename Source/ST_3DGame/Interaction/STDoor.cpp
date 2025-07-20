// Fill out your copyright notice in the Description page of Project Settings.


#include "STDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ASTDoor::ASTDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	InteractionPromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptWidget"));
	InteractionPromptWidget->SetupAttachment(RootComponent);
	InteractionPromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ASTDoor::BeginPlay()
{
	Super::BeginPlay();

	InteractionPromptWidget->SetVisibility(false);

	if (DoorRotationCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("UpdateDoorRotation"));
		DoorTimeline.AddInterpFloat(DoorRotationCurve, ProgressFunction);
	}
}

void ASTDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorTimeline.TickTimeline(DeltaTime);
}

void ASTDoor::Interact_Implementation(AActor* Interactor)
{
	bIsOpen = !bIsOpen;
	if (bIsOpen)
	{
		DoorTimeline.Play();
	}
	else
	{
		DoorTimeline.Reverse();
	}
}

void ASTDoor::ShowInteractionPrompt_Implementation()
{
	InteractionPromptWidget->SetVisibility(true);
}

void ASTDoor::HideInteractionPrompt_Implementation()
{
	InteractionPromptWidget->SetVisibility(false);
}

void ASTDoor::UpdateDoorRotation(float Value)
{
	FRotator NewRotation = FRotator(0.f, Value * 90.f, 0.f);
	DoorMesh->SetRelativeRotation(NewRotation);
}

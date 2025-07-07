// Fill out your copyright notice in the Description page of Project Settings.


#include "STPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

ASTPlayerController::ASTPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  LookAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr)
{
}

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (HUDWidgetClass)
	{
		if (UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass))
		{
			HUDWidget->AddToViewport();
		}
	}
}

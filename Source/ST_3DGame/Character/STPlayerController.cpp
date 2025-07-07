// Fill out your copyright notice in the Description page of Project Settings.


#include "STPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "ST_3DGame/GameModes/STGameState.h"

ASTPlayerController::ASTPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  LookAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr),
	  HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr)
{
}

UUserWidget* ASTPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
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
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	if (ASTGameState* ASTGameState = GetWorld() ? GetWorld()->GetGameState<class ASTGameState>() : nullptr)
	{
		ASTGameState->UpdateHUD();
	}
}

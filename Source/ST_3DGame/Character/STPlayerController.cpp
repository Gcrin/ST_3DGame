// Fill out your copyright notice in the Description page of Project Settings.


#include "STPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ST_3DGame/System/STGameInstance.h"

ASTPlayerController::ASTPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  LookAction(nullptr),
	  JumpAction(nullptr),
	  SprintAction(nullptr),
	  HUDWidgetClass(nullptr),
	  HUDWidgetInstance(nullptr),
	  MainMenuWidgetClass(nullptr),
	  MainMenuWidgetInstance(nullptr)
{
}

UUserWidget* ASTPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASTPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}
}

void ASTPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (!MainMenuWidgetClass)
	{
		return;
	}

	MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->AddToViewport();

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}

	if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
	{
		if (bIsRestart)
		{
			ButtonText->SetText(FText::FromString(TEXT("재시작")));

			if (UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim")))
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(
				MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USTGameInstance* STGameInstance = Cast<USTGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(
						FText::FromString(FString::Printf(TEXT("총합 점수: %d"), STGameInstance->GetTotalScore())));
				}
			}
		}
		else
		{
			ButtonText->SetText(FText::FromString(TEXT("시작")));
		}
	}
}

void ASTPlayerController::StartGame()
{
	if (USTGameInstance* STGameInstance = Cast<USTGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		STGameInstance->SetCurrentLevelIndex(0);
		STGameInstance->SetTotalScore(0);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
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

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

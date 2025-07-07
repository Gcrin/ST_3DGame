// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ST_3DGAME_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
		
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	// HUD 표시
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	// 메인 메뉴 표시
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	// 게임 시작
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	
protected:
	virtual void BeginPlay() override;
};

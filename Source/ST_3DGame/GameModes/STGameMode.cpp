// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameMode.h"

#include "ST_3DGame/STGameState.h"
#include "ST_3DGame/Character/STCharacter.h"
#include "ST_3DGame/Character/STPlayerController.h"


ASTGameMode::ASTGameMode()
{
	DefaultPawnClass = ASTCharacter::StaticClass();
	PlayerControllerClass = ASTPlayerController::StaticClass();
	GameStateClass = ASTGameState::StaticClass();
}

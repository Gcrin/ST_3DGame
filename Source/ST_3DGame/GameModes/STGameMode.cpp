// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameMode.h"

#include "ST_3DGame/Character/STCharacter.h"


ASTGameMode::ASTGameMode()
{
	DefaultPawnClass = ASTCharacter::StaticClass();	
}

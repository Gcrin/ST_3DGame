// Fill out your copyright notice in the Description page of Project Settings.


#include "STCharacter.h"

// Sets default values
ASTCharacter::ASTCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

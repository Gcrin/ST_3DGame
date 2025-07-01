// Fill out your copyright notice in the Description page of Project Settings.


#include "STItem.h"

ASTItem::ASTItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneRoot);

	RotationSpeed = 90.f;
}

void ASTItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASTItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaSeconds, 0.f));
}

float ASTItem::GetRotationSpeed() const
{
	return RotationSpeed;
}

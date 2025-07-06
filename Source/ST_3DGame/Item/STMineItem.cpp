// Fill out your copyright notice in the Description page of Project Settings.


#include "STMineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASTMineItem::ASTMineItem(): ExplosionDelay(5.0f), ExplosionRadius(300.0f), ExplosionDamage(30.0f)
{
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->SetupAttachment(Scene);
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ASTMineItem::ActivateItem(AActor* Activator)
{
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ASTMineItem::Explode, ExplosionDelay);
}

void ASTMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		}
	}
	DestroyItem();
}

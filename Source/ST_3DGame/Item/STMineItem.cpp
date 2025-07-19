// Fill out your copyright notice in the Description page of Project Settings.


#include "STMineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASTMineItem::ASTMineItem()
{
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->SetupAttachment(Scene);
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ParticleDuration = ExplosionDelay;
}

void ASTMineItem::ActivateItem(AActor* Activator)
{
	if (bHasExploded)
	{
		return;
	}
	Super::ActivateItem(Activator);

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ASTMineItem::Explode, ExplosionDelay);
	bHasExploded = true;
	StaticMesh->AddLocalOffset(FVector(0.0f, 0.0f, -5.0f));
}

void ASTMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(),
		                                                    GetActorRotation(), false);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

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

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr WeakParticle = Particle;
						
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]()
			{
					if (WeakParticle.IsValid())
					{
							WeakParticle->DestroyComponent();
					}
			},
			2.0f,
			false
		);
	}
}

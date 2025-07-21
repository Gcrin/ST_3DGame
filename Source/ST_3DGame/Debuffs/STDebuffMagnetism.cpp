// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffMagnetism.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ST_3DGame/Character/STCharacter.h"
#include "ST_3DGame/Item/STBaseItem.h"
#include "ST_3DGame/Item/STCoinItem.h"

void USTDebuffMagnetism::OnApply(ASTCharacter* Target)
{
	check(Target != nullptr);

	if (USphereComponent* MagnetField = Target->GetMagnetFieldComponent())
	{
		MagnetField->Activate();
		MagnetField->SetSphereRadius(MagnetRadius);

		// 2. 오버랩 이벤트에 우리 함수들을 바인딩합니다.
		MagnetField->OnComponentBeginOverlap.AddDynamic(this, &USTDebuffMagnetism::OnFieldBeginOverlap);
		MagnetField->OnComponentEndOverlap.AddDynamic(this, &USTDebuffMagnetism::OnFieldEndOverlap);

		TArray<AActor*> InitiallyOverlappingActors;
		MagnetField->GetOverlappingActors(InitiallyOverlappingActors, ASTBaseItem::StaticClass());

		for (AActor* OverlappedActor : InitiallyOverlappingActors)
		{
			if (ASTBaseItem* Item = Cast<ASTBaseItem>(OverlappedActor))
			{
				AttractedItems.AddUnique(Item);
			}
		}
	}
}

void USTDebuffMagnetism::OnRemove(ASTCharacter* Target)
{
	check(Target != nullptr);

	if (USphereComponent* MagnetField = Target->GetMagnetFieldComponent())
	{
		MagnetField->Deactivate();
		MagnetField->SetSphereRadius(0.f);

		MagnetField->OnComponentBeginOverlap.RemoveDynamic(this, &USTDebuffMagnetism::OnFieldBeginOverlap);
		MagnetField->OnComponentEndOverlap.RemoveDynamic(this, &USTDebuffMagnetism::OnFieldEndOverlap);
	}
	AttractedItems.Empty();
}

void USTDebuffMagnetism::OnFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (ASTBaseItem* Item = Cast<ASTBaseItem>(OtherActor))
	{
		AttractedItems.AddUnique(Item);
	}
}

void USTDebuffMagnetism::OnFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASTBaseItem* Item = Cast<ASTBaseItem>(OtherActor))
	{
		AttractedItems.Remove(Item);
	}
}

void USTDebuffMagnetism::OnTick(ASTCharacter* Target, float DeltaTime)
{
	check(Target != nullptr);

	for (int32 i = AttractedItems.Num() - 1; i >= 0; --i)
	{
		if (AttractedItems[i].IsValid())
		{
			if (ASTCoinItem* Item = Cast<ASTCoinItem>(AttractedItems[i].Get()))
			{
				FVector ItemLocation = Item->GetActorLocation();
				FVector TargetLocation = Target->GetActorLocation();

				if (FVector::DistSquared(Item->GetActorLocation(), TargetLocation) > FMath::Square(10.f))
				{
					FVector NewLocation = UKismetMathLibrary::VInterpTo(
						ItemLocation,
						TargetLocation,
						DeltaTime,
						AttractionSpeed
					);
					Item->SetActorLocation(NewLocation);
				}
			}
			
		}
		else
		{
			AttractedItems.RemoveAt(i);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "STSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

ASTSpawnVolume::ASTSpawnVolume() : TotalChance(0.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

AActor* ASTSpawnVolume::SpawnRandomItem()
{
	if (const FSTItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

void ASTSpawnVolume::BeginPlay()
{
	InitializeSpawnData();
	Super::BeginPlay();
}

FVector ASTSpawnVolume::GetRandomPointInVolume() const
{
	constexpr int32 MaxLoop = 10;
	
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	for (int32 Loop = 0; Loop < MaxLoop; ++Loop)
	{
		FVector StartPoint = BoxOrigin + FVector(
			FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
			FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
			BoxExtent.Z
		);

		FHitResult HitResult;
		FVector EndPoint = StartPoint - FVector(0, 0, BoxExtent.Z * 2);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_WorldStatic))
		{
			return HitResult.Location;
		}
	}
	return BoxOrigin;
}

const FSTItemSpawnRow* ASTSpawnVolume::GetRandomItem() const
{
	// 캐싱된 데이터가 없으면 null 반환
	if (SpawnableItems.IsEmpty() || TotalChance <= 0.0f)
	{
		return nullptr;
	}

	const float RandValue = FMath::FRand() * TotalChance;

	// 이진 탐색(UpperBound)을 사용하여 효율적으로 아이템 검색
	// RandValue보다 큰 첫 번째 누적 확률 값의 인덱스를 찾습니다.
	const int32 SelectedIndex = Algo::UpperBound(CumulativeChances, RandValue);

	// 해당 인덱스가 유효한지 확인
	if (SpawnableItems.IsValidIndex(SelectedIndex))
	{
		return &SpawnableItems[SelectedIndex];
	}

	return nullptr;
}

AActor* ASTSpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	return GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
}

void ASTSpawnVolume::InitializeSpawnData()
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDatatable이 설정되지 않았습니다 %s"), *GetName());
		return;
	}

	// 기존 데이터 초기화
	SpawnableItems.Empty();
	CumulativeChances.Empty();
	TotalChance = 0.0f;

	static const FString ContextString(TEXT("ItemSpawnContext"));
	TArray<FSTItemSpawnRow*> AllRows;
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return;

	// 데이터 테이블을 순회하며 누적 확률 계산 및 아이템 저장
	for (FSTItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->SpawnChance > 0.0f)
		{
			TotalChance += Row->SpawnChance;
			SpawnableItems.Add(*Row);
			CumulativeChances.Add(TotalChance);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STItemSpawnRow.h"
#include "STSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class ST_3DGAME_API ASTSpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ASTSpawnVolume();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	TObjectPtr<UBoxComponent> SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UDataTable> ItemDataTable;

	// 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
	UFUNCTION(BlueprintCallable, Category="Spawning")
	FVector GetRandomPointInVolume() const;
	const FSTItemSpawnRow* GetRandomItem() const;
	// 특정 아이템 클래스를 스폰하는 함수
	UFUNCTION(BlueprintCallable, Category="Spawning")
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

private:
	TArray<FSTItemSpawnRow> SpawnableItems;
	TArray<float> CumulativeChances;
	float TotalChance;
	void InitializeSpawnData();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class ST_3DGAME_API ASTSpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	ASTSpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	USceneComponent* Scene;
	// 스폰 영역을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
	UBoxComponent* SpawningBox;

	// 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
	UFUNCTION(BlueprintCallable, Category="Spawning")
	FVector GetRandomPointInVolume() const;
	// 특정 아이템 클래스를 스폰하는 함수
	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnItem(TSubclassOf<AActor> ItemClass);
};

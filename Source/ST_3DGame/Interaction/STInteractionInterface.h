// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "STInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class USTInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class ST_3DGAME_API ISTInteractionInterface
{
	GENERATED_BODY()

public:
	// 플레이어가 이 객체와 상호작용할 때 호출될 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Interactor);

	// 플레이어가 이 객체를 바라보기 시작했을 때 호출될 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void ShowInteractionPrompt();

	// 플레이어가 이 객체에서 시선을 뗐을 때 호출될 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void HideInteractionPrompt();
};

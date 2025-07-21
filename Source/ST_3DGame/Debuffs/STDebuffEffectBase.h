// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "STDebuffEffectBase.generated.h"

class ASTCharacter;

UCLASS(Blueprintable, Abstract)
class ST_3DGAME_API USTDebuffEffectBase : public UObject
{
	GENERATED_BODY()

public:
	// 디버프가 처음 적용될 때 호출
	virtual void OnApply(ASTCharacter* Target)
	{
	}

	// 디버프가 지속되는 동안 매 프레임 호출
	virtual void OnTick(ASTCharacter* Target, float DeltaTime)
	{
	}

	// 디버프가 끝날 때 호출
	virtual void OnRemove(ASTCharacter* Target)
	{
	}
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "STDebuffPoison.h"

#include "Engine/DamageEvents.h"
#include "ST_3DGame/Character/STCharacter.h"

void USTDebuffPoison::OnTick(ASTCharacter* Target, float DeltaTime)
{
	check(Target != nullptr);

	TickAccumulator += DeltaTime;

	if (TickAccumulator >= 1.0f)
	{
		Target->TakeDamage(DamagePerSecond, FDamageEvent(), nullptr, nullptr);
		TickAccumulator -= 1.0f;
	}
}

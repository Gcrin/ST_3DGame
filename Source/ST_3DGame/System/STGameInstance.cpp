// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameInstance.h"

USTGameInstance::USTGameInstance(): TotalScore(0), CurrentLevelIndex(0)
{
}

void USTGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("총 점수 업데이트 : %d"), TotalScore);
}

void USTGameInstance::SetTotalScore(int32 NewTotalScore)
{
	TotalScore = NewTotalScore;
}

int32 USTGameInstance::GetTotalScore() const
{
	return TotalScore;
}

int32 USTGameInstance::GetCurrentLevelIndex() const
{
	return CurrentLevelIndex;
}

void USTGameInstance::SetCurrentLevelIndex(int32 NewLevelIndex)
{
	CurrentLevelIndex = NewLevelIndex;
}

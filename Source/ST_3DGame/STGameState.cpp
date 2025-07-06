// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameState.h"

ASTGameState::ASTGameState(): Score(0)
{
}

int32 ASTGameState::GetScore() const
{
	return Score;
}

void ASTGameState::AddScore(int32 Amount)
{
	Score += Amount;
}
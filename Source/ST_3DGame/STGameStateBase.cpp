// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameStateBase.h"

ASTGameStateBase::ASTGameStateBase(): Score(0)
{
}

int32 ASTGameStateBase::GetScore() const
{
	return Score;
}

void ASTGameStateBase::AddScore(int32 Amount)
{
	Score += Amount;
}

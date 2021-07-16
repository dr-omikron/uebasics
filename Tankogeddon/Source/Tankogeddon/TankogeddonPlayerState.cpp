// Fill out your copyright notice in the Description page of Project Settings.


#include "TankogeddonPlayerState.h"

ATankogeddonPlayerState::ATankogeddonPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
}

int32 ATankogeddonPlayerState::GetScores() const
{
	return CurrentScore;
}
void ATankogeddonPlayerState::AddScores(int32 Scores)
{
	CurrentScore += Scores;
}
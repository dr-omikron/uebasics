// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TankogeddonPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankogeddonPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATankogeddonPlayerState();
	
	UFUNCTION(BlueprintCallable, Category = "Scores")
	void AddScores(int32 Scores);

	UFUNCTION(BlueprintPure, Category = "Scores")
	int32 GetScores() const;

private:
	UPROPERTY()
	int32 CurrentScore = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ScorableInterfase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UScorableInterfase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKOGEDDON_API IScorableInterfase
{
	GENERATED_BODY()

public:
	virtual int32 GetDestroyScore() const = 0;
};

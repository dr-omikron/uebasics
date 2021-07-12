// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "UObject/Interface.h"
#include "DamageTakerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageTakerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKOGEDDON_API IDamageTakerInterface
{
	GENERATED_BODY()
			
public:
	virtual void TakeDamage(FDamageData DamageData) = 0;
};

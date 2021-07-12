// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankogeddonGameModeBase.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankogeddonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATankogeddonGameModeBase();
	UFUNCTION(BlueprintCallable, Category = "Game|Projectile")
	class AProjectile* GetOrCreateProjectile(TSubclassOf<AProjectile> ProjectileClass, FTransform NewTransform);
	
protected:
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
	TArray<AProjectile*> ProjectilesPool;

	UFUNCTION()
	void OnProjectileStopped(AProjectile* InProjectile);
};

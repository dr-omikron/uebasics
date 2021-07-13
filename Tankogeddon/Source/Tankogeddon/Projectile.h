// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Delegates/DelegateCombinations.h>
#include "Projectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileStopped, class AProjectile*, InProjectile);

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	void Start();
	void StartSpecial();
	void Stop();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1.f;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Special|Movement")
	float MoveRangeSpecial = 6.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Special|Fire")
	float TimeToExplosion = 2.f;

	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnProjectileStopped OnStopped;
	
	
protected:
	FTimerHandle MovementTimerHandle;
	FTimerHandle StopTimerHandle;

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Move();

	UFUNCTION()
	void MoveSpecial();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	float FlewDistance = 0.f;
};

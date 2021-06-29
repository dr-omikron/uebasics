// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"


class UArrowComponent;
class UStaticMeshComponent;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CanonMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireRateMachineGun = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int MachineGunShootsRate = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int MaxAmmo = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int MaxAmmoSpecial = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	ECannonType Type = ECannonType::FireProjectile;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireMachineGun();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireMachineGunSpecial();
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void MaxAmmoReload();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void MaxAmmoSpecialReload();
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool IsReadyToFire() const;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool IsAnyAmmo();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool IsAnyAmmoSpecial();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Timer();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	void Reload();

private:
	int AmmoStock;
	int AmmoStockSpecial;
	int MachineGunShoots;
	bool bIsReadyToFire = false;
	bool bIsAnyAmmo = false;
	bool bIsAnyAmmoSpecial = false;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle MachineGunTimerHandle;
};

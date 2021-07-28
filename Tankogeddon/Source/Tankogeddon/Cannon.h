// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class AProjectile;
class UArrowComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UAudioComponent;
class UMatineeCameraShake;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UAudioComponent* AudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> ShootShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* HitEffect;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireMachineGunRate = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int32 FireMachineGunShots = 3;
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	float FireDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int32 MaxAmmo = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int32 MaxAmmoMachineGun = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int32 MaxAmmoSpecial = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int32 MaxAmmoRockets = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	int32 MaxAmmoTrace = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire Property")
	TSubclassOf<AProjectile> ProjectileSpecialClass;
				
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void AddAmmo(int32 AmmoQuantity);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void AddAmmoRockets(int32 AmmoQuantity);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void AddAmmoTrace(int32 AmmoQuantity);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void AddAmmoSpecial(int32 AmmoQuantity);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void AddAmmoMachineGun(int32 AmmoQuantity);
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireMachineGun();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	AProjectile* GetProjectile();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	AProjectile* GetProjectileSpecial();
	
	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Timer();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void SpawnProjectileSpecial();

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void StartEffects();

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void CameraShake();

	UFUNCTION(BlueprintCallable, Category = "Visibility")
	bool IsReadyToFireCannonClass();

	UFUNCTION(BlueprintCallable, Category = "Visibility")
	void SetIsActive(bool bInIsActive);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	void Reload();

private:
	int32 AmmoStock = 0;
	int32 AmmoStockTrace = 0;
	int32 AmmoStockRockets = 0;
	int32 AmmoStockSpecial = 5;
	int32 AmmoStockMachineGun = 0;
	int32 CurrentFireMachineGunShots = 0;
	bool bIsReloading = false;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle MachineGunTimerHandle;
};

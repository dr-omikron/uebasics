// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DamageTakerInterface.h"
#include "GameStruct.h"
#include "ScorableInterfase.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class UHealthComponent;
class ACannon;
class UBoxComponent;
class UArrowComponent;
class UParticleSystem;
class USoundBase;
class UMatineeCameraShake;

UCLASS()
class TANKOGEDDON_API ATurret : public APawn, public IDamageTakerInterface, public IScorableInterfase
{
    GENERATED_BODY()
public:
    ATurret();
	
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* BodyMesh;
	
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* TurretMesh;
	
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UArrowComponent* CannonSetupPoint;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;
	
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBoxComponent* HitCollider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    TSubclassOf<ACannon> CannonClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* DestructionEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    USoundBase* DestructionSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    TSubclassOf<UMatineeCameraShake> DestructionShake;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accurency = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scores")
    int32 DestroyedScore = 5;

	const FString BodyMeshPath = "StaticMesh'/Game/Meshes/turret_base.turret_base'";
    const FString TurretMeshPath = "StaticMesh'/Game/Meshes/turret_gun.turret_gun'";

	UFUNCTION()
    virtual bool TakeDamage(FDamageData DamageData) override;

    UFUNCTION()
    virtual int32 GetDestroyScore() const override;
	
protected:
    virtual void BeginPlay() override;
    virtual void Destroyed() override;
    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
	UFUNCTION(BlueprintPure)
    bool CanFire() const;
    UFUNCTION(BlueprintCallable)
    bool QueryCanSeePlayer();
    void Fire();
	
private:
    UPROPERTY()
    ACannon* Cannon;

    UPROPERTY()
    APawn* PlayerPawn;

    UFUNCTION()
    void Die();

    UFUNCTION()
    void DamageTaken(float DamageTaken);
};

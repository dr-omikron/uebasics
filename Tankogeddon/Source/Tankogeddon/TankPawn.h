// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTakerInterface.h"
#include "GameStruct.h"
#include "ScorableInterfase.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UHealthComponent;
class UBoxComponent;
class UArrowComponent;
class ACannon;
class ATankPlayerController;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTakerInterface, public IScorableInterfase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Cannon")
	UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float TurretRotationSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scores")
	int32 DestroyedScore = 10;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TArray<TSubclassOf<ACannon>> DefaultCannonClasses;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InMoveForwardAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float InMoveRightAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InMoveRightAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void FireSpecial();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void CycleCannon();

	UFUNCTION(BlueprintCallable, Category = "Cannon")
	void SetupCannon(TSubclassOf<ACannon> NewCannon);

	UFUNCTION(BlueprintPure, Category = "Cannon")
	ACannon* GetCurrentCannon() const;

	UFUNCTION()
	virtual bool TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	virtual int32 GetDestroyScore() const override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

	UFUNCTION()
	void AddHealth(float AddiditionalHealthValue);
	
private:
	UPROPERTY()
	ATankPlayerController* TankController = nullptr;
	UPROPERTY()
	TArray<ACannon*> CannonSlots;
	UPROPERTY()
	uint8 CurrentCannon = INDEX_NONE;
	
	float CurrentMoveForwardAxis = 0.f;
	float CurrentMoveRightAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;
	float TargetMoveForwardAxis = 0.f;
	float TargetMoveRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;

	void SetupCannonInternal(int32 SlotIndex, TSubclassOf<ACannon> NewCannonClass);
};

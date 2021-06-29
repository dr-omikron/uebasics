// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UArrowComponent;
class ACannon;
class ATankPlayerController;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<ACannon> CannonClass;
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
	
private:
	
	void SetupCannon();
	UPROPERTY()
	ATankPlayerController* TankController = nullptr;
	UPROPERTY()
	ACannon* Cannon;
	float CurrentMoveForwardAxis = 0.f;
	float CurrentMoveRightAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;
	float TargetMoveForwardAxis = 0.f;
	float TargetMoveRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;
};

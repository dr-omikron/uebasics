// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATankPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSecond) override;

	UFUNCTION(BlueprintPure, Category = "Input")
	FVector GetMousePos() const;
	
private:
	void MoveForward(float InMoveForwardAxisValue);
	void MoveRight(float InMoveRightAxisValue);
	void RotateRight(float InRotateRightAxisValue);
	void Fire();
	void FireSpecial();
	FVector CasheMousePos;
	UPROPERTY()
	class ATankPawn* TankPawn;
};

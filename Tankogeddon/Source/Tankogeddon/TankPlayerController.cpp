// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "TankPawn.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
}

void ATankPlayerController::MoveForward(float InMoveForwardAxisValue)
{
	if(TankPawn)
	{
		TankPawn->MoveForward(InMoveForwardAxisValue);
	}
}

void ATankPlayerController::MoveRight(float InMoveRightAxisValue)
{
	if(TankPawn)
	{
		TankPawn->MoveRight(InMoveRightAxisValue);
	}
}
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}
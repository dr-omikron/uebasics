// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("Fire Special", IE_Pressed, this, &ATankPlayerController::FireSpecial);
}

void ATankPlayerController::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
	
	FVector MousePositionScreen, MouseDirectionScreen;
	DeprojectMousePositionToWorld(MousePositionScreen, MouseDirectionScreen);
	FVector MousePosition = MousePositionScreen.Z / MouseDirectionScreen.Z * MouseDirectionScreen * -1 + MousePositionScreen;
	if(TankPawn)
	{
		FVector TankPosition = TankPawn->GetActorLocation();
		FVector MouseDir = MousePosition - TankPosition;
		CasheMousePos.Z = 0;
		CasheMousePos.Normalize();
		CasheMousePos = TankPosition + MouseDir * 1000.f;
		
		/*UE_LOG(LogTemp, Warning, TEXT("MousePosition = %f %f %f MouseDirection = %f %f %f"),
			MousePositionScreen.X, MousePositionScreen.Y, MousePositionScreen.Z, MouseDirectionScreen.X, MouseDirectionScreen.Y, MouseDirectionScreen.Z);*/
	}
	
	
}

FVector ATankPlayerController::GetMousePos() const
{
	return CasheMousePos;
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
void ATankPlayerController::RotateRight(float InRotateRightAxisValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(InRotateRightAxisValue);
	}
}
void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}
void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}
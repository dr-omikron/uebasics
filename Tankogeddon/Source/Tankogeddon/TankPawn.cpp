// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankBodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankTurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TankSpringArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TankCamera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + GetActorForwardVector() * MoveSpeed * CurrentMoveForwardAxis * DeltaTime;
	NewLocation += GetActorRightVector() * MoveSpeed * CurrentMoveRightAxis * DeltaTime;
	SetActorLocation(NewLocation);
}

void ATankPawn::MoveForward(float InMoveForwardAxisValue)
{
	CurrentMoveForwardAxis = InMoveForwardAxisValue;
}

void ATankPawn::MoveRight(float InMoveRightAxisValue)
{
	CurrentMoveRightAxis = InMoveRightAxisValue;
}



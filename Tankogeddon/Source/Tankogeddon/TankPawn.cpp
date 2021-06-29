// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "TankPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body Mesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret Mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon Setup Point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Tank Spring Arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Tank Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentMoveForwardAxis = FMath::Lerp(CurrentMoveForwardAxis, TargetMoveForwardAxis, MoveSensitivity);
	CurrentMoveRightAxis = FMath::Lerp(CurrentMoveRightAxis, TargetMoveRightAxis, MoveSensitivity);
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + GetActorForwardVector() * MoveSpeed * CurrentMoveForwardAxis * DeltaTime;
	NewLocation += GetActorRightVector() * MoveSpeed * CurrentMoveRightAxis * DeltaTime;
	SetActorLocation(NewLocation);

	CurrentRotateRightAxis = FMath::Lerp(CurrentRotateRightAxis, TargetRotateRightAxis, MoveSensitivity);
	FRotator CurrentRotation = GetActorRotation();
	FRotator NewRotation { 0.f ,CurrentRotation.Yaw + RotationSpeed * CurrentRotateRightAxis * DeltaTime, 0.f };
	SetActorRotation(NewRotation);
	if(TankController)
	{
		FVector TurretLookAtPoint = CurrentLocation + TankController->GetMousePos() * 1000.f;
		FVector TurretCurrentLocation = TurretMesh->GetComponentLocation();
		FRotator TargetTurretRotation = UKismetMathLibrary::FindLookAtRotation(TurretCurrentLocation, TurretLookAtPoint);
		FRotator CurrentTurretRotation = TurretMesh->GetComponentRotation();
		TargetTurretRotation.Pitch = CurrentTurretRotation.Pitch;
		TargetTurretRotation.Roll = CurrentTurretRotation.Roll;
		TurretLookAtPoint.Z = 0;
		DrawDebugLine(GetWorld(), TurretCurrentLocation, TurretLookAtPoint, FColor::Cyan, false, 0.f, 0, 5.f);
		TurretMesh->SetWorldRotation(FMath::Lerp(CurrentTurretRotation, TargetTurretRotation, TurretRotationSensitivity));
	}
}

void ATankPawn::MoveForward(float InMoveForwardAxisValue)
{
	TargetMoveForwardAxis = InMoveForwardAxisValue;
}

void ATankPawn::MoveRight(float InMoveRightAxisValue)
{
	TargetMoveRightAxis = InMoveRightAxisValue;
}

void ATankPawn::RotateRight(float InMoveRightAxisValue)
{
	TargetRotateRightAxis = InMoveRightAxisValue;
}

void ATankPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if(Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::SetupCannon()
{
	if(Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}
	if(CannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}



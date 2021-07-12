// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "TankPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
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
	DefaultCannonClasses.Add(ACannon::StaticClass());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::DamageTaked);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
	if(DefaultCannonClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Num of DefautCannonClasses should be larger than zero!"))
		DefaultCannonClasses.Add(ACannon::StaticClass());
	}
	CannonSlots.SetNumZeroed(DefaultCannonClasses.Num());
	CurrentCannon = 0;
	for(int32 CurrentSlot = 0; CurrentSlot < CannonSlots.Num(); ++CurrentSlot)
	{
		SetupCannonInternal(CurrentSlot, DefaultCannonClasses[CurrentSlot]);
	}
	
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
	if(ACannon* Cannon = GetCurrentCannon())
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if(ACannon* Cannon = GetCurrentCannon())
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::CycleCannon()
{
	if(ACannon* Cannon = GetCurrentCannon())
	{
		Cannon->SetIsActive(false);
	}
	if(!CannonSlots.IsValidIndex(++CurrentCannon))
	{
		CurrentCannon = 0;
	}
	if(ACannon* Cannon = GetCurrentCannon()) 
	{
		Cannon->SetIsActive(true);
	}
	GEngine->AddOnScreenDebugMessage(6, 1.f, FColor::Emerald, FString::Printf(TEXT("Selected Cannon %d"), CurrentCannon));
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> NewCannon)
{
	SetupCannonInternal(CurrentCannon, NewCannon);
}

ACannon* ATankPawn::GetCurrentCannon() const
{
	check(CannonSlots.IsValidIndex(CurrentCannon));
	return CannonSlots[CurrentCannon];
}

void ATankPawn::SetupCannonInternal(int32 SlotIndex, TSubclassOf<ACannon> NewCannonClass)
{
	check(CannonSlots.IsValidIndex(SlotIndex));
	ACannon*& CannonSlot = CannonSlots[SlotIndex];
	if (CannonSlot)
	{
		CannonSlot->Destroy();
		CannonSlot = nullptr;
	}
	if (NewCannonClass)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CannonSlot = GetWorld()->SpawnActor<ACannon>(NewCannonClass, Params);
		CannonSlot->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
		CannonSlot->SetIsActive(CurrentCannon == SlotIndex);
	}
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankPawn::Die()
{
	Destroy();
}

void ATankPawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
	GEngine->AddOnScreenDebugMessage(11, 3.f, FColor::Red, FString::Printf(TEXT("Tank Health: %f%%"), HealthComponent->GetHealthState()));
}

void ATankPawn::AddHealth(float AddiditionalHealthValue)
{
	HealthComponent->AddHealth(AddiditionalHealthValue);
}

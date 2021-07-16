// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
	
	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDie.AddUObject(this, &ATurret::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATurret::DamageTaken);

}

bool ATurret::TakeDamage(FDamageData DamageData) 
{
	HealthComponent->TakeDamage(DamageData);
	if(HealthComponent->GetHealth() <= 0)
	{
		return true;
	}
	return false;
}

int32 ATurret::GetDestroyScore() const
{
	return DestroyedScore;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Destroyed()
{
	if (Cannon)
		Cannon->Destroy();
}

void ATurret::Targeting()
{
	if (IsPlayerInRange() && QueryCanSeePlayer())
	{
		RotateToPlayer();
	}

	if (CanFire() && Cannon && Cannon->IsReadyToFireCannonClass())
	{
		Fire();
	}
}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
	if(PlayerPawn)
	{
		return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
	}
	return false;
}

bool ATurret::CanFire() const
{
	if(PlayerPawn)
	{
		FVector TargetingDir = TurretMesh->GetForwardVector();
		FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
		DirToPlayer.Normalize();
		float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));
		return AimAngle <= Accurency;
	}
	return false;
}

bool ATurret::QueryCanSeePlayer()
{
	bool bResult = false;
	FCollisionQueryParams Params(FName(TEXT("Enemy Eyes")), true, this);
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = false;
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, CannonSetupPoint->GetComponentLocation(), PlayerPawn->GetActorLocation(), ECC_Visibility, Params))
	{
		if (HitResult.Actor.Get())
		{
			bResult = HitResult.Actor.Get() == PlayerPawn;
		}
	}
	return bResult;
}

void ATurret::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void ATurret::Die()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionEffect, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (DestructionShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DestructionShake);
		}
	}
	Destroy();
}

void ATurret::DamageTaken(float DamageTaken)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage: %f"), *GetName(), HealthComponent->GetHealth());
}

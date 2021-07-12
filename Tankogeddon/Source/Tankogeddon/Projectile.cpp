// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include "DamageTakerInterface.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
}

void AProjectile::Start()
{
	FlewDistance = 0.f;
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
}

void AProjectile::StartSpecial()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::MoveSpecial, MoveRate, true, MoveRate);
	GetWorld()->GetTimerManager().SetTimer(StopTimerHandle, this, &AProjectile::Stop, TimeToExplosion, false, TimeToExplosion);
}

void AProjectile::Stop()
{
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	OnStopped.Broadcast(this);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();
	AActor* OwnerByOwner = MyOwner != nullptr ? MyOwner->GetOwner() : nullptr;
	if(OtherActor != MyOwner && OtherActor != OwnerByOwner)
	{
		IDamageTakerInterface* DamageTakerActor = Cast<IDamageTakerInterface>(OtherActor);
		if(DamageTakerActor)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = GetInstigator();
			DamageData.DamageMaker = this;

			DamageTakerActor->TakeDamage(DamageData);
		}
		else
		{
			OtherActor->Destroy();
		}
	}
	Stop();
}

void AProjectile::Move()
{
	FVector MoveOffset = GetActorForwardVector() * MoveSpeed * MoveRate;
	FVector NextPosition = GetActorLocation() + MoveOffset;
	SetActorLocation(NextPosition);
	FlewDistance += MoveOffset.Size2D();
	if(FlewDistance >= MoveRange)
	{
		Stop();
	}
}

void AProjectile::MoveSpecial()
{
	FVector NextPosition = GetActorLocation() + ((GetActorForwardVector() * MoveSpeed) - ((GetActorUpVector() * MoveSpeed / MoveRangeSpecial))) * MoveRate;
	if (NextPosition.Z <= 0.f)
	{
		return;
	}
	SetActorLocation(NextPosition);
}
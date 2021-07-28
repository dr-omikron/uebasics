// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "TankogeddonPlayerState.h"
#include "DamageTakerInterface.h"
#include "ScorableInterfase.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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
	if (bIsExplode)
	{
		Explode();
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	OnStopped.Broadcast(this);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != GetInstigator() && !OtherActor->GetClass()->IsChildOf(StaticClass()))
	{
		bool bWasDestroyed = false;
		IDamageTakerInterface* DamageTakerActor = Cast<IDamageTakerInterface>(OtherActor);
		if(DamageTakerActor)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = GetInstigator();
			DamageData.DamageMaker = this;

			bWasDestroyed = DamageTakerActor->TakeDamage(DamageData);
		}
		else
		{
			UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if(Primitive && Primitive->IsSimulatingPhysics())
			{
				FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
				ForceVector.Normalize();
				Primitive->AddImpulse(ForceVector * PushForce, NAME_None, true);
			}
			else
			{
				Stop();
				return;
			}
		}
		IScorableInterfase* Scorable = Cast<IScorableInterfase>(OtherActor);
		if (Scorable && bWasDestroyed && GetInstigator())
		{
			
			ATankogeddonPlayerState* PlayerState = GetInstigator()->GetController()->GetPlayerState<ATankogeddonPlayerState>();
			if(PlayerState != nullptr)
			{
				PlayerState->AddScores(Scorable->GetDestroyScore());
				GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::Cyan, FString::Printf(TEXT("Scores is: %d"), PlayerState->GetScores()));
			}
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

void AProjectile::DamageCheck(AActor* OtherActor)
{
	bool bWasDestroyed = false;
	IDamageTakerInterface* DamageTakerActor = Cast<IDamageTakerInterface>(OtherActor);
	if (DamageTakerActor)
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = GetOwner();
		DamageData.DamageMaker = this;

		bWasDestroyed = DamageTakerActor->TakeDamage(DamageData);
	}
	else
	{
		UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (Mesh)
		{
			if (Mesh->IsSimulatingPhysics())
			{
				FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
				ForceVector.Normalize();
				Mesh->AddForce(ForceVector * PushForce, NAME_None, true);
			}
		}
	}
	IScorableInterfase* Scorable = Cast<IScorableInterfase>(OtherActor);
	if (Scorable && bWasDestroyed && GetInstigator())
	{

		ATankogeddonPlayerState* PlayerState = GetInstigator()->GetController()->GetPlayerState<ATankogeddonPlayerState>();
		if (PlayerState != nullptr)
		{
			PlayerState->AddScores(Scorable->GetDestroyScore());
			GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::Cyan, FString::Printf(TEXT("Scores is: %d"), PlayerState->GetScores()));
		}
	}
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
}

void AProjectile::Explode()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";

	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;

	bool bSweepResult = GetWorld()->SweepMultiByChannel(AttackHit, StartPos, EndPos, Rotation, ECollisionChannel::ECC_Visibility, Shape, Params);

	GetWorld()->DebugDrawTraceTag = "Explode Trace";

	if (bSweepResult)
	{
		for (FHitResult HitResult : AttackHit)
		{
			AActor* OtherActor = HitResult.GetActor();
			if (!OtherActor)
			{
				continue;
			}
			DamageCheck(OtherActor);
		}
	}
}
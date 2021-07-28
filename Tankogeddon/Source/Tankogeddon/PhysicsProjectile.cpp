// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"

#include "DrawDebugHelpers.h"
#include "PrijectilePhysicsComponent.h"
#include "Kismet/KismetMathLibrary.h"

APhysicsProjectile::APhysicsProjectile()
{
    PhysicsComponent = CreateDefaultSubobject<UPrijectilePhysicsComponent>(TEXT("Physics Component"));

    TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
    TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
    MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
    CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0.f);

	if (bShowTrajectory)
    {
        for (FVector Position : CurrentTrajectory)
        {
            DrawDebugSphere(GetWorld(), Position, 5, 8, FColor::Purple, true, 1, 0, 2);
        }
    }

    TragectoryPointIndex = 0;
    Super::Start();
}

void APhysicsProjectile::Move()
{
    FVector CurrentMoveVector = CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
    CurrentMoveVector.Normalize();
    FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
    SetActorLocation(NewLocation);

	if (FVector::Distance(NewLocation, CurrentTrajectory[TragectoryPointIndex]) <= MoveAccurency)
    {
        TragectoryPointIndex++;

		if (TragectoryPointIndex >= CurrentTrajectory.Num())
        {
            Stop();
        }
        else
        {
            FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
            SetActorRotation(NewRotation);
        }
    }
}

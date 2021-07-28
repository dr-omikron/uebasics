// Fill out your copyright notice in the Description page of Project Settings.


#include "PrijectilePhysicsComponent.h"

// Sets default values for this component's properties
UPrijectilePhysicsComponent::UPrijectilePhysicsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FVector> UPrijectilePhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue)
{
	TArray<FVector> Trajectory;
	FVector GravityVector(0, 0, Gravity);

	for (float Time = 0; Time < MaxTime; Time += TimeStep)
	{
		FVector Position = StartPos + Velocity * Time + GravityVector * Time * Time / 2;
		if(Position.Z <= MinZValue)
		{
			break;
		}
		Trajectory.Add(Position);
	}

	return Trajectory;
}

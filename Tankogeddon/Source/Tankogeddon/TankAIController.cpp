// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!OurTank)
	{
		Initialize();
	}
	
	if(!OurTank || CurrentPatrolPointIndex == INDEX_NONE)
	{
		return;
	}

	OurTank->MoveForward(1);

	FVector CurrentPoint = CashedPatrollingPoints[CurrentPatrolPointIndex];
	FVector PawnLocation = OurTank->GetActorLocation();
	if(FVector::Distance(CurrentPoint, PawnLocation) <= OurTank->MovementAccurancy)
	{
		if(++CurrentPatrolPointIndex >= CashedPatrollingPoints.Num())
		{
			CurrentPatrolPointIndex = 0;
		}
	}
	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	FVector ForwardDirection = OurTank->GetActorForwardVector();
	FVector RightDirection = OurTank->GetActorRightVector();

	float ForwardAngleCos = FVector::DotProduct(ForwardDirection, MoveDirection);
	float RightAngleCos = FVector::DotProduct(RightDirection, MoveDirection);

	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(ForwardAngleCos));
	float RightAngle = FMath::RadiansToDegrees (FMath::Acos(RightAngleCos));

	float RotationValue = 0;
	if(ForwardAngle > 5)
	{
		RotationValue = 1;
	}
	if(RightAngle > 90)
	{
		RotationValue = -RotationValue;
	}
	OurTank->RotateRight(RotationValue);

	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(PlayerTank)
	{
		bool bCanSeePlayer = false;
		FVector PlayerTankLocation = PlayerTank->GetActorLocation();
		if(FVector::Distance(PlayerTankLocation, PawnLocation) < TargetingRange)
		{
			FCollisionQueryParams Params(FName(TEXT("Enemy Eyes")), true, this);
			Params.AddIgnoredActor(OurTank);
			Params.bReturnPhysicalMaterial = false;
			FHitResult HitResult;
			
			if(GetWorld()->LineTraceSingleByChannel(HitResult, OurTank->GetEyesPosition(), PlayerTankLocation, ECC_Visibility, Params))
			{
				if(HitResult.Actor.Get())
				{
					bCanSeePlayer = HitResult.Actor.Get() == PlayerTank;
				}
			}
			
		}
		if(bCanSeePlayer)
		{
			OurTank->SetTurretLookAtPoint(PlayerTankLocation);

			FVector DirectionToPlayer = PlayerTankLocation - PawnLocation;
			DirectionToPlayer.Normalize();
			FVector TurretDirection = OurTank->GetTurretDirection();
			float AngleToPlayer = FMath::Acos(FVector::DotProduct(TurretDirection, DirectionToPlayer));
			if (FMath::RadiansToDegrees(AngleToPlayer) < Accurency)
			{
				OurTank->Fire();
			}
		}
	}
}

void ATankAIController::Initialize()
{
	OurTank = Cast<ATankPawn>(GetPawn());
	if (OurTank && OurTank->PatrollingPoints.Num() > 0)
	{
		CurrentPatrolPointIndex = 0;
		for (ATargetPoint* Point : OurTank->PatrollingPoints)
		{
			if (Point)
			{
				CashedPatrollingPoints.Add(Point->GetActorLocation());
			}
		}
	}
}

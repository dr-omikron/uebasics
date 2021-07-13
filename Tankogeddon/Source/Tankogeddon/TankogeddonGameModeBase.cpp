// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankogeddonGameModeBase.h"

#include "Projectile.h"


ATankogeddonGameModeBase::ATankogeddonGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ATankogeddonGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATankogeddonGameModeBase::OnProjectileStopped(AProjectile* InProjectile)
{
	check(InProjectile);
	InProjectile->SetActorHiddenInGame(true);
	InProjectile->SetActorEnableCollision(false);
	ProjectilesPool.Push(InProjectile);
}

AProjectile* ATankogeddonGameModeBase::GetOrCreateProjectile(TSubclassOf<AProjectile> ProjectileClass, FTransform NewTransform)
{

	AProjectile* NewProjectile = nullptr;

	if (ProjectilesPool.Num() > 0)
	{
	    for(auto Iterator = ProjectilesPool.CreateIterator(); Iterator; ++Iterator)
		{
			if (ProjectilesPool[Iterator.GetIndex()] && ProjectilesPool[Iterator.GetIndex()]->GetClass() == ProjectileClass)
			{
				NewProjectile = ProjectilesPool[Iterator.GetIndex()];
				Iterator.RemoveCurrent();
				UE_LOG(LogTemp, Log, TEXT("Got projectile from pool. Pool size is %d"), ProjectilesPool.Num());
				break;
			}
		}
	}
	if(NewProjectile)
	{
		NewProjectile->SetActorHiddenInGame(false);
		NewProjectile->SetActorEnableCollision(true);
		NewProjectile->SetActorTransform(NewTransform);
	}
	else
	{
		NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, NewTransform);
		NewProjectile->OnStopped.AddDynamic(this, &ATankogeddonGameModeBase::OnProjectileStopped);
	}
	return NewProjectile;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "MapLoader.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ATankFactory::ATankFactory()
{
 	PrimaryActorTick.bCanEverTick = false;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	DestroyedBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destroyed Building Mesh"));
	DestroyedBuildingMesh->SetupAttachment(SceneComp);
		
	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(SceneComp, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaked);

	DestroyEffect = CreateDefaultSubobject<UParticleSystemComponent>("Destroy Effect");
	DestroyEffect->SetupAttachment(SceneComp);
	DestroyAudioEffect = CreateDefaultSubobject<UAudioComponent>("Destroy Audio Effect");
	DestroyAudioEffect->SetupAttachment(SceneComp);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	BuildingMesh->SetVisibility(true);
	DestroyedBuildingMesh->SetVisibility(false);

	if (LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(false);
	}

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::Destroyed()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

bool ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	if (HealthComponent->GetHealth() <= 0)
	{
		return true;
	}
	return false;
}

void ATankFactory::Die()
{
	if(LinkedMapLoader)
	{
		LinkedMapLoader->SetIsActivated(true);
		UE_LOG(LogTemp, Warning, TEXT("Green"))
	}
	DestroyEffect->ActivateSystem();
	DestroyAudioEffect->Play();
	BuildingMesh->SetVisibility(false);
	DestroyedBuildingMesh->SetVisibility(true);
	SetActorEnableCollision(false);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::SpawnNewTank()
{
	if(SpawnTankClass && TankWayPoints.Num() > 0)
	{
		FTransform SpawnTranform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
		ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTranform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewTank->PatrollingPoints = TankWayPoints;
		NewTank->FinishSpawning(SpawnTranform);
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "DamageTakerInterface.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "ScorableInterfase.h"
#include "TankogeddonGameModeBase.h"
#include "TankogeddonPlayerState.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
ACannon::ACannon()
{
 	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;
	CanonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon Mesh"));
	CanonMesh->SetupAttachment(SceneComp);
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(CanonMesh);
}

void ACannon::AddAmmo(int32 AmmoQuantity)
{
	AmmoStock = FMath::Clamp(0, MaxAmmo, AmmoStock + AmmoQuantity);
}

void ACannon::AddAmmoRockets(int32 AmmoQuantity)
{
	AmmoStockRockets = FMath::Clamp(0, MaxAmmoRockets, AmmoStockRockets + AmmoQuantity);
}

void ACannon::AddAmmoTrace(int32 AmmoQuantity)
{
	AmmoStockTrace = FMath::Clamp(0, MaxAmmoTrace, AmmoStockTrace + AmmoQuantity);
}

void ACannon::AddAmmoSpecial(int32 AmmoQuantity)
{
	AmmoStockSpecial = FMath::Clamp(0, MaxAmmoSpecial, AmmoStockSpecial + AmmoQuantity);
}

void ACannon::AddAmmoMachineGun(int32 AmmoQuantity)
{
	AmmoStockMachineGun = FMath::Clamp(0, MaxAmmoMachineGun, AmmoStockMachineGun + AmmoQuantity);
}

void ACannon::Fire()
{
	if (Type == ECannonType::FireProjectile)
	{
		if (!bIsReloading && AmmoStockMachineGun > 0)
		{
			CurrentFireMachineGunShots = FireMachineGunShots;
			AmmoStockMachineGun--;
			GetWorld()->GetTimerManager().SetTimer(MachineGunTimerHandle, this, &ACannon::FireMachineGun, 1.f / FireMachineGunRate, true);
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Machine Gun Ammo: %d"), AmmoStockMachineGun));
			Timer();
		}
		else if (IsReadyToFireCannonClass())
		{
			AmmoStock--;
			GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire - Projectile"));
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Ammo: %d"), AmmoStock));
			SpawnProjectile();
			Timer();
		}
		if(AmmoStock <= 0)
		{
			GEngine->AddOnScreenDebugMessage(7, 1.f, FColor::Red, FString::Printf(TEXT("Ammo Stock is empty")));
		}
	}
		
	if (Type == ECannonType::FireTrace)
	{
		if (IsReadyToFireCannonClass())
		{
			AmmoStockTrace--;
			GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire - Trace"));
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Trace Gun Ammo: %d"), AmmoStockTrace));
			FHitResult HitResult;
			FVector StartPoint = ProjectileSpawnPoint->GetComponentLocation();
			FVector EndPoint = StartPoint + ProjectileSpawnPoint->GetForwardVector() * FireRange;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("FireTrace"), true, this);
			TraceParams.bTraceComplex = true;
			TraceParams.bReturnPhysicalMaterial = false;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility, TraceParams))
			{
				DrawDebugLine(GetWorld(), StartPoint, HitResult.Location, FColor::Red, false, 0.5f, 0, 10);
				if (HitResult.Actor.IsValid())
				{
					bool bWasDestroyed = false;
					AActor* HitActor = Cast<AActor>(HitResult.Actor);
					IScorableInterfase* Scorable = Cast<IScorableInterfase>(HitResult.Actor);
					if(IDamageTakerInterface* Damagable = Cast<IDamageTakerInterface>(HitResult.Actor))
					{
						FDamageData DamageData;
						DamageData.DamageMaker = this;
						DamageData.Instigator = GetInstigator();
						DamageData.DamageValue = FireDamage;
						bWasDestroyed = Damagable->TakeDamage(DamageData);
					}
					else
					{
						HitActor->Destroy();
						bWasDestroyed = true;
					}
					if (Scorable && bWasDestroyed && GetInstigator())
					{
						ATankogeddonPlayerState* PlayerState = GetInstigator()->GetPlayerState<ATankogeddonPlayerState>();
						if (PlayerState)
						{
							PlayerState->AddScores(Scorable->GetDestroyScore());
							GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::Cyan, FString::Printf(TEXT("Scores is: %d"), PlayerState->GetScores()));
						}
					}
				}
			}
			else
			{
				DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Green, false, 0.5f, 0, 10);
			}
			Timer();
		}
		if (AmmoStockTrace <= 0)
		{
			GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Red, FString::Printf(TEXT("Ammo Stock Trace is empty")));
		}
	}
	if (Type == ECannonType::FireRockets)
	{
		if (IsReadyToFireCannonClass())
		{
			AmmoStockRockets--;
			GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire - Rockets"));
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Rockets : %d"), AmmoStockRockets));
			SpawnProjectile();
			Timer();
		}
		if (AmmoStockRockets <= 0)
		{
			GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Red, FString::Printf(TEXT("Ammo Stock Rockets is empty")));
		}
	}
}

void ACannon::FireSpecial()
{
	if (!bIsReloading && AmmoStockSpecial > 0)
	{
		AmmoStockSpecial--;
		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire - Special"));
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Special Ammo: %d"), AmmoStockSpecial));
		SpawnProjectileSpecial();
		Timer();
	}
	if (AmmoStockSpecial <= 0)
	{
		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Red, FString::Printf(TEXT("Ammo Stock Special is empty")));
	}
}

void ACannon::FireMachineGun()
{
	if (--CurrentFireMachineGunShots <= 0)
	{
		GetWorldTimerManager().ClearTimer(MachineGunTimerHandle);
	}
	GEngine->AddOnScreenDebugMessage(9, 0.1, FColor::Green, TEXT("Fire - MachineGun"));
	SpawnProjectile();
}

AProjectile* ACannon::GetProjectile()
{
	FTransform NewProjectileTransform;
	NewProjectileTransform.SetLocation(ProjectileSpawnPoint->GetComponentLocation());
	NewProjectileTransform.SetRotation(ProjectileSpawnPoint->GetComponentRotation().Quaternion());
	ATankogeddonGameModeBase* GameMode = Cast<ATankogeddonGameModeBase>(GetWorld()->GetAuthGameMode());
	AProjectile* Projectile = GameMode->GetOrCreateProjectile(ProjectileClass, NewProjectileTransform);
	Projectile->SetInstigator(GetInstigator());
	Projectile->MoveRange = FireRange;
	return Projectile;
}

void ACannon::Timer()
{
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}

void ACannon::SpawnProjectile()
{
	GetProjectile()->Start();
}

void ACannon::SpawnProjectileSpecial()
{
	GetProjectile()->StartSpecial();
}

bool ACannon::IsReadyToFireCannonClass()
{
	if(Type == ECannonType::FireProjectile)
	{
		return !bIsReloading && AmmoStock > 0;
	}
	if(Type == ECannonType::FireTrace)
	{
		return !bIsReloading && AmmoStockTrace > 0;
	}
	if (Type == ECannonType::FireRockets)
	{
		return !bIsReloading && AmmoStockRockets > 0;
	}
	return false;
}

void ACannon::SetIsActive(bool bInIsActive)
{
	CanonMesh->SetVisibility(bInIsActive);
};

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
	if(Type == ECannonType::FireProjectile)
	{
		AddAmmo(MaxAmmo);
		AddAmmoMachineGun(MaxAmmoMachineGun);
	}
	if (Type == ECannonType::FireRockets)
	{
		AddAmmoRockets(MaxAmmoRockets);
	}
	if (Type == ECannonType::FireTrace)
	{
		AddAmmoTrace(MaxAmmoTrace);
	}
}

void ACannon::EndPlay(const EEndPlayReason::Type Reason)
{
	GetWorldTimerManager().ClearTimer(MachineGunTimerHandle);
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	MachineGunTimerHandle.Invalidate();
	ReloadTimerHandle.Invalidate();
	Super::EndPlay(Reason);
}

void ACannon::Reload()
{
	bIsReloading = false;
}



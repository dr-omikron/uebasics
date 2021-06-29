// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include <string>

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

void ACannon::Fire()
{
	IsAnyAmmo();
	
	if(!bIsReadyToFire || !bIsAnyAmmo)
	{
		return;
	}
	
	bIsReadyToFire = false;

	std::string AmmoStockDebugStr = std::to_string(AmmoStock);
	
	if(Type == ECannonType::FireProjectile)
	{
		AmmoStock--;
		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire - Projectile"));
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, AmmoStockDebugStr.c_str());
	}
	if(Type == ECannonType::FireTrace)
	{
		AmmoStock--;
		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire - Trace"));
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, AmmoStockDebugStr.c_str());
	}
	if(Type == ECannonType::FireMachineGun)
	{
		MachineGunShoots = MachineGunShootsRate;
		AmmoStock--;
		GetWorld()->GetTimerManager().SetTimer(MachineGunTimerHandle, this, &ACannon::FireMachineGun, 1.f / FireRateMachineGun, true);
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, AmmoStockDebugStr.c_str());
	}
	Timer();
}

void ACannon::MaxAmmoReload()
{
	AmmoStock = MaxAmmo;
	bIsAnyAmmo = true;
}

void ACannon::MaxAmmoSpecialReload()
{
	AmmoStockSpecial = MaxAmmoSpecial;
	bIsAnyAmmoSpecial = true;
}

void ACannon::FireSpecial()
{
	IsAnyAmmoSpecial();
	
	if (!bIsReadyToFire || !bIsAnyAmmoSpecial)
	{
		return;
	}

	bIsReadyToFire = false;

	std::string AmmoStockSpecialDebugStr = std::to_string(AmmoStockSpecial);

	if (Type == ECannonType::FireProjectile)
	{
		AmmoStockSpecial--;
		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Red, TEXT("Fire Special - Projectile"));
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Blue, AmmoStockSpecialDebugStr.c_str());
	}
	if (Type == ECannonType::FireTrace)
	{
		AmmoStockSpecial--;
		GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Green, TEXT("Fire Special - Trace"));
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Blue, AmmoStockSpecialDebugStr.c_str());
	}
	if (Type == ECannonType::FireMachineGun)
	{
		MachineGunShoots = MachineGunShootsRate;
		AmmoStockSpecial--;
		GetWorld()->GetTimerManager().SetTimer(MachineGunTimerHandle, this, &ACannon::FireMachineGunSpecial, 1.f / FireRateMachineGun, true);
		GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Blue, AmmoStockSpecialDebugStr.c_str());
	}
	Timer();
}

void ACannon::FireMachineGunSpecial()
{
	if (--MachineGunShoots <= 0)
	{
		GetWorldTimerManager().ClearTimer(MachineGunTimerHandle);
	}
	GEngine->AddOnScreenDebugMessage(9, 0.1, FColor::Red, TEXT("Fire Special - MachineGun"));
}

void ACannon::FireMachineGun()
{
	if (--MachineGunShoots <= 0)
	{
		GetWorldTimerManager().ClearTimer(MachineGunTimerHandle);
	}
	GEngine->AddOnScreenDebugMessage(9, 0.1, FColor::Green, TEXT("Fire - MachineGun"));
}

bool ACannon::IsReadyToFire() const
{
	return bIsReadyToFire;
}

bool ACannon::IsAnyAmmo() 
{
	if(!AmmoStock)
	{
		bIsAnyAmmo = false;
	}
	return bIsAnyAmmo;
}

bool ACannon::IsAnyAmmoSpecial()
{
	if (!AmmoStockSpecial)
	{
		bIsAnyAmmoSpecial = false;
	}
	return bIsAnyAmmoSpecial;
}

void ACannon::Timer()
{
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
	MaxAmmoReload();
	MaxAmmoSpecialReload();
}

void ACannon::EndPlay(const EEndPlayReason::Type Reason)
{
	MachineGunTimerHandle.Invalidate();
	ReloadTimerHandle.Invalidate();
	Super::EndPlay(Reason);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}



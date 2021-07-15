#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStruct.generated.h"

UENUM(BlueprintType)

enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use Projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use Trace"),
	FireRockets = 2 UMETA(DisplayName = "Use Rockets"),
};

UENUM(BlueprintType)

enum class EBonusBoxType : uint8
{
	AddHealth = 0 UMETA(DisplayName = "Add Heath"),
	AddAmmo = 1 UMETA(DisplayName = "Add Ammo"),
	AddRockets = 2 UMETA(DisplayName = "Add Rockets"),
	AddMachineGunAmmo = 3 UMETA(DisplayName = "Add Machine Gun Ammo"),
	AddSpecialAmmo = 4 UMETA(DisplayName = "Add Special Ammo"),
	AddTraceAmmo = 4 UMETA(DisplayName = "Add Trace Ammo")
};

USTRUCT(BlueprintType)
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue;

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	AActor* DamageMaker;
};

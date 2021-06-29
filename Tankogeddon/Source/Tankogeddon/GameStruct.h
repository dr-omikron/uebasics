#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStruct.generated.h"

UENUM(BlueprintType)

enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use Projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use Trace"),
	FireMachineGun = 2 UMETA(DisplayName = "Use Machine Gun")
};
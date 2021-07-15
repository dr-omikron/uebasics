
#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT(UHealthComponent, FOnDie)
	DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float)

public:	
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values")
	float MaxHealth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values")
	float CurrentHealth = 0;

public:
	FOnDie OnDie;
	FOnHealthChanged OnDamaged;
		
	void TakeDamage(FDamageData DamageData);

	float GetHealth() const;

	float GetHealthState() const;

	void AddHealth(float AddiditionalHealthValue);
};

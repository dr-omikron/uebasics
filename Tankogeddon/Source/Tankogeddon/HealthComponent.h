
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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
	float MaxHealth = 10;

	UPROPERTY()
	float CurrentHealth;

public:
	FOnDie OnDie;
	FOnHealthChanged OnDamaged;
		
	void TakeDamage(FDamageData DamageData);

	float GetHealth() const;

	float GetHealthState() const;

	void AddHealth(float AddiditionalHealthValue);
};

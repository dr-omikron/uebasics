// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DamageTakerInterface.h"
#include "GameStruct.h"
#include "HealthComponent.h"
#include "ScorableInterfase.h"
#include "GameFramework/Actor.h"
#include "Mine.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;
class UMatineeCameraShake;


UCLASS()
class TANKOGEDDON_API AMine : public AActor, public IScorableInterfase, public IDamageTakerInterface
{
	GENERATED_BODY()
	
public:	
	AMine();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MineMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* DestructionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	USoundBase* DestructionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> DestructionShake;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scores")
	int32 DestroyedScore = 2;

	UFUNCTION()
	virtual int32 GetDestroyScore() const override;

	UFUNCTION()
	virtual bool TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	void Die();

	void DamageTaken(float DamageTaken);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	
protected:
	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

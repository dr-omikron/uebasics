// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameStruct.h"
#include "GameFramework/Actor.h"
#include "BonusBox.generated.h"

class ATankPawn;
class ACannon;
UCLASS()
class TANKOGEDDON_API ABonusBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABonusBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BonusBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	EBonusBoxType BoxType;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float HealthAmount = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoAmount = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Special Ammo")
	int32 SpecialAmmoAmount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rockets")
	int32 RocketsAmount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Machine Gun Ammo")
	int32 MachineGunAmmoAmount = 5;

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

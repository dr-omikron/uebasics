// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

class USceneComponent;
class UStaticMeshComponen;
class UBoxComponent;
class UPointLightComponent;
class UMaterial;

UCLASS()
class TANKOGEDDON_API AMapLoader : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* GatesMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TriggerCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UMaterial* MaterialRed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UMaterial* MaterialGreen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName LoadLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	bool bIsActivated = false;

	AMapLoader();

	UFUNCTION(BlueprintCallable, Category = "Map Loader")
	void SetIsActivated(bool NewIsActivated);

protected:
	virtual void BeginPlay() override;

	void SetActiveMaterial();

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonBox.generated.h"

class ACannon;

UCLASS()
class TANKOGEDDON_API ACannonBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called when the game starts or when spawned
	ACannonBox();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CannonBoxMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ACannon> CannonClass; 

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

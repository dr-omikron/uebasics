// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ScorableInterfase.h"
#include "GameFramework/Actor.h"
#include "Mine.generated.h"

UCLASS()
class TANKOGEDDON_API AMine : public AActor, public IScorableInterfase
{
	GENERATED_BODY()
	
public:	
	AMine();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MineMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scores")
	int32 DestroyedScore = 2;

	UFUNCTION()
	virtual int32 GetDestroyScore() const override;
	
protected:
	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

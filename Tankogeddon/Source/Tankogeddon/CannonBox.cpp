// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonBox.h"
#include "TankPawn.h"

// Sets default values
ACannonBox::ACannonBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	CannonBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon Box Mesh"));
	CannonBoxMesh->SetupAttachment(SceneComponent);
	CannonBoxMesh->OnComponentBeginOverlap.AddDynamic(this, &ACannonBox::OnMeshOverlapBegin);
	CannonBoxMesh->SetCollisionProfileName(FName("OverlapAll"));
	CannonBoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CannonBoxMesh->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ACannonBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACannonBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* Tank = Cast<ATankPawn>(OtherActor);
	if(Tank)
	{
		Tank->SetupCannon(CannonClass);
		Destroy();
	}
}



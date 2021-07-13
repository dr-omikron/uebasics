// Fill out your copyright notice in the Description page of Project Settings.


#include "BonusBox.h"

#include "Cannon.h"
#include "TankPawn.h"

// Sets default values
ABonusBox::ABonusBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	BonusBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus Box Mesh"));
	BonusBox->SetupAttachment(SceneComponent);
	BonusBox->OnComponentBeginOverlap.AddDynamic(this, &ABonusBox::OnMeshOverlapBegin);
	BonusBox->SetCollisionProfileName(FName("OverlapAll"));
	BonusBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BonusBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ABonusBox::BeginPlay()
{
	Super::BeginPlay();
}

void ABonusBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* MyTank = Cast<ATankPawn>(OtherActor);
	
	if(BoxType == EBonusBoxType::AddHealth)
	{
		if (MyTank)
		{
			MyTank->AddHealth(HealthAmount);
			Destroy();
		}
		UE_LOG(LogTemp, Warning, TEXT("Tank overlapped health box"))
	}
	if (BoxType == EBonusBoxType::AddAmmo)
	{
		if (MyTank && MyTank->GetCurrentCannon()->Type == ECannonType::FireProjectile)
		{
			MyTank->GetCurrentCannon()->AddAmmo(AmmoAmount);
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Add %d Ammo: "), AmmoAmount));
			Destroy();
		}
	}
	if (BoxType == EBonusBoxType::AddAmmo)
	{
		if (MyTank && MyTank->GetCurrentCannon()->Type == ECannonType::FireTrace)
		{
			MyTank->GetCurrentCannon()->AddAmmoTrace(AmmoAmount);
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Add %d Trace Ammo"), AmmoAmount));
			Destroy();
		}
	}
	if (BoxType == EBonusBoxType::AddSpecialAmmo)
	{
		if (MyTank)
		{
			MyTank->GetCurrentCannon()->AddAmmoSpecial(SpecialAmmoAmount);
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Add %d Special Ammo: "), SpecialAmmoAmount));
			Destroy();
		}
	}
	if (BoxType == EBonusBoxType::AddAmmo)
	{
		if (MyTank && MyTank->GetCurrentCannon()->Type == ECannonType::FireRockets)
		{
			MyTank->GetCurrentCannon()->AddAmmoRockets(RocketsAmount);
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Add %d Rockets: "), RocketsAmount));
			Destroy();
		}
	}
	if (BoxType == EBonusBoxType::AddMachineGunAmmo)
	{
		if (MyTank && MyTank->GetCurrentCannon()->Type == ECannonType::FireProjectile)
		{
			MyTank->GetCurrentCannon()->AddAmmoMachineGun(MachineGunAmmoAmount);
			GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Magenta, FString::Printf(TEXT("Add %d Machine Gun Ammo: "), MachineGunAmmoAmount));
			Destroy();
		}
	}
}


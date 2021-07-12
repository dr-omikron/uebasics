
#include "Mine.h"

#include "TankPawn.h"


AMine::AMine()
{
 	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus Box Mesh"));
	MineMesh->SetupAttachment(SceneComponent);
	MineMesh->OnComponentBeginOverlap.AddDynamic(this, &AMine::OnMeshOverlapBegin);
	MineMesh->SetCollisionProfileName(FName("OverlapAll"));
	MineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MineMesh->SetGenerateOverlapEvents(true);
}


void AMine::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* MyTank = Cast<ATankPawn>(OtherActor);
	if(MyTank)
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = GetInstigator();
		DamageData.DamageMaker = this;

		MyTank->TakeDamage(DamageData);
	}
	Destroy();
}


#include "Mine.h"

#include "TankPawn.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AMine::AMine()
{
 	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mine Mesh"));
	MineMesh->SetupAttachment(SceneComponent);
	MineMesh->OnComponentBeginOverlap.AddDynamic(this, &AMine::OnMeshOverlapBegin);
	MineMesh->SetCollisionProfileName(FName("OverlapAll"));
	MineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MineMesh->SetGenerateOverlapEvents(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &AMine::Die);
	HealthComponent->OnDamaged.AddUObject(this, &AMine::DamageTaken);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(MineMesh);
}


int32 AMine::GetDestroyScore() const
{
	return DestroyedScore;
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
	Die();
}

bool AMine::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	if (HealthComponent->GetHealth() <= 0)
	{
		return true;
	}
	return false;
}

void AMine::DamageTaken(float DamageTaken)
{
	UE_LOG(LogTemp, Warning, TEXT("Mine %s taked damage: %f"), *GetName(), HealthComponent->GetHealth());
}

void AMine::Die()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionEffect, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());
	if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (DestructionShake)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DestructionShake);
			}
	}
	Destroy();
}
#include "ProjectilePoolComponent.h"
#include "Projectile.h"

UProjectilePoolComponent::UProjectilePoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectilePoolComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectilePoolComponent::CreatePool(const TSubclassOf<AProjectile> ActorSpawnClass, int MaxSize = 50)
{
	ActorClass = ActorSpawnClass;
	for (int i = 0; i < MaxSize; i++)
	{
		TObjectPtr<AProjectile> Actor = CreateActor();
		
		// Disable immediately
		Actor->SetActorEnableCollision(false);
		Actor->SetActorHiddenInGame(true);
		
		InsertToPool(Actor);
	}
}

void UProjectilePoolComponent::InsertToPool(TObjectPtr<AProjectile> Actor)
{
	Actor->SetActorEnableCollision(false);
	Actor->SetActorHiddenInGame(true);
	Pool.Add(Actor);
}

TObjectPtr<AProjectile> UProjectilePoolComponent::GetActorFromPool()
{

	if (Pool.IsEmpty())
	{
		InsertToPool(CreateActor());
	}
	TObjectPtr<AProjectile> PooledActor = Pool.Pop();
	PooledActor->SetActorEnableCollision(true);
	PooledActor->SetActorHiddenInGame(false);
	return PooledActor;
}

TObjectPtr<AProjectile> UProjectilePoolComponent::CreateActor()
{
	FVector Vector = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	TObjectPtr<AProjectile> Actor = GetWorld()->SpawnActor<AProjectile>(ActorClass, Vector, Rotation);

	return Actor;
}




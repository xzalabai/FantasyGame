#include "Projectile.h"
#include "Enemy.h"
#include "FireWeapon.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(CollisionComponent);
	
	// TODO: set from BP
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//InitialLifeSpan = 1.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddUniqueDynamic(this, &AProjectile::OnHit);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AProjectile::ReturnToPool, LifeSpan, false, LifeSpan);
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("[AProjectile] OnHit. %s in bone: %s"), *OtherActor->GetName(), *Hit.BoneName.ToString());
	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->OnReceivedHit(Hit.ImpactPoint, Hit.Location, nullptr, 50);
	}
	int8 RandomIndex = FMath::RandRange(0, DecalMaterials.Num() - 1);

	PlaceDecal(DecalMaterials[RandomIndex], OtherComponent, Hit.ImpactPoint, Hit.Normal);
	ReturnToPool();
}

void AProjectile::ReturnToPool()
{
	UE_LOG(LogTemp, Display, TEXT("[AProjectile] ReturnToPool"));
	GetWorldTimerManager().ClearTimer(TimerHandle);
	OwnerWeapon->ReturnToPool(this);
}
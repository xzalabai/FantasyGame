#include "Projectile.h"
#include "Enemy.h"
#include "FireWeapon.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	// TODO: WHY THIS IS IN TUT?
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(CollisionComponent);
	
	// TODO: set from BP
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->MaxSpeed = 8000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	InitialLifeSpan = 1.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddUniqueDynamic(this, &AProjectile::OnHit);
	SetLifeSpan(InitialLifeSpan);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("[AProjectile] OnHit."));
	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->OnReceivedHit(Hit.ImpactPoint, nullptr, 50);
	}
	int8 RandomIndex = FMath::RandRange(0, DecalMaterials.Num() - 1);

	PlaceDecal(DecalMaterials[RandomIndex], OtherComponent, Hit.ImpactPoint, Hit.Normal);

	//TObjectPtr<AFireWeapon> Weapon = Cast<AFireWeapon>(GetOwner());
	//Weapon->ReturnToPool(this);
	Destroy();
}
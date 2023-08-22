#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"

AFireWeapon::AFireWeapon()
{	
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
}

void AFireWeapon::BeginPlay()
{
	Super::BeginPlay();
	Muzzle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void AFireWeapon::EnableOverlappingEvents(bool Enable)
{
    return;    
}

void AFireWeapon::FireFromGun()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), Muzzle->GetComponentRotation(), SpawnParams);
		if (Projectile)
		{
            UE_LOG(LogTemp, Warning, TEXT("Fire shot"));
			Projectile->FireInDirection(Muzzle->GetForwardVector());
		}
	}
}

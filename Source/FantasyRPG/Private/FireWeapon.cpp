#include "FireWeapon.h"
#include "Projectile.h"


void AFireWeapon::EnableOverlappingEvents(bool Enable)
{
    return;    
}

void AFireWeapon::FireFromGun(FVector &ProjectileSpawnLocation, FRotator &ProjectileSpawnRotation, FVector &ProjectileDirection)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, SpawnParams);
		if (Projectile)
		{
            UE_LOG(LogTemp, Warning, TEXT("Fire shot"));
			Projectile->FireInDirection(ProjectileDirection);
		}
	}
}

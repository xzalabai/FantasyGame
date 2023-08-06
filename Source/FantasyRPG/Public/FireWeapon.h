#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponInterface.h"
#include "FireWeapon.generated.h"

class AProjectile;

UCLASS()

class FANTASYRPG_API AFireWeapon : public AItem, public IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void EnableOverlappingEvents(bool Enable) override;
	void FireFromGun(FVector &ProjectileSpawnLocation, FRotator &ProjectileSpawnRotation, FVector &ProjectileDirection);
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
};

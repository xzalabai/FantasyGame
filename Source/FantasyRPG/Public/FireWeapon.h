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
protected:
	virtual void BeginPlay() override;
public:
	AFireWeapon();
	virtual void EnableOverlappingEvents(bool Enable) override;
	void FireFromGun();
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;
};

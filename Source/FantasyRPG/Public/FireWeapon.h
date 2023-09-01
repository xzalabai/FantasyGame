#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FireWeapon.generated.h"

class AProjectile;
class UAnimInstance;
class AHeroCharacter;

UCLASS()

class FANTASYRPG_API AFireWeapon : public AWeapon
{

	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	AFireWeapon();
	virtual void EnableOverlappingEvents(bool Enable) override;
	void FireFromGun();
	virtual void InitiateAttack(AHeroCharacter& Character, UAnimInstance& AnimInstance) override;
	virtual void AttackMontageStarted() override;
	virtual void AttackMontageEnded() override;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;
	AHeroCharacter* GetCharacter();

};



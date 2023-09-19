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
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	TArray<FName> ReloadAnimationSequenceName;
public:
	AFireWeapon();
	virtual void EnableOverlappingEvents(bool Enable) override;
	void FireFromWeapon();
	void ReloadWeapon();
	virtual void PerformMontage(AHeroCharacter* Character, UAnimInstance* AnimInstance) override;
	virtual void AttackMontageStarted() override;
	virtual void AttackMontageEnded() override;
	virtual void PerformActionOnNotify() override;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmoCapacity;
	UPROPERTY(EditDefaultsOnly)
	int32 AmmoInMagazine;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmoInMagazine;
	UPROPERTY(EditDefaultsOnly)
	int32 AmmoCapacity;
};





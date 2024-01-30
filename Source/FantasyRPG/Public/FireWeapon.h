#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FireWeapon.generated.h"

class AProjectile;
class UAnimInstance;
class AHeroCharacter;
class UProjectilePoolComponent;

UCLASS()

class FANTASYRPG_API AFireWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AFireWeapon();
	void FireFromWeapon();
	bool ReloadWeapon();
	void OnMouseRelease();
	virtual void PerformMontage(class UAnimInstance* AnimInstance, FName MontageName = "", class UAnimMontage* AnimMontage = nullptr) override;
	virtual void AttackMontageStarted() override;
	virtual void AttackMontageEnded() override;
	virtual void PerformActionOnNotify() override;
	void ReturnToPool(TObjectPtr<AProjectile> Projectile);
	virtual void UpdateItemWithAttributes() override;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = AnimationProperties)
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, Category = AnimationProperties)
	TArray<FName> ReloadAnimationSequenceName;
	UPROPERTY()
	TObjectPtr<UProjectilePoolComponent> ProjectilePool;
	virtual void OnItemEquipped(AHeroCharacter* MainCharacter) override;
	UPROPERTY(EditDefaultsOnly)
	UDataTable* FireWeaponDataTable;
private:
	void ClearWeaponTimer();
	bool CalculateShotEndPosition(const FVector& Start, const FVector& End, FHitResult& HitResult);
	FVector CreateShotDispersion(const FVector OriginalTarget, const int16 bIsAiming);
	FTimerHandle WeaponTimerHandle;
	FTimerHandle ClearTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;
	int32 AmmoInMagazine = 0;		// Current ammo in magazine
	int32 MaxAmmoInMagazine = 0;	// Maximum size of ammo in magazine
	int32 AmmoCapacity = 0;			// Maximum total ammo
	float FireRate = 0;
	bool bMouseReleased = false;	
};







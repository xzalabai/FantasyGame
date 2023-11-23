#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

class AFireWeapon;
class UMaterialInterface;

UCLASS()
class FANTASYRPG_API AProjectile : public AActor
{
	GENERATED_BODY()

public:	
	AProjectile();
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Options")
	UStaticMeshComponent* ProjectileMeshComponent;
	UPROPERTY()
	AFireWeapon* OwnerWeapon;
	void FireInDirection(const FVector& ShootDirection);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Options")
	float LifeSpan = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Options")
	float Strength = 50;
	UFUNCTION(BlueprintImplementableEvent)
	void PlaceDecal(UMaterialInterface* Material, UPrimitiveComponent* HitComponent, FVector HitLocation, FVector HitNormal);
private:
	void ReturnToPool();
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnywhere, Category="Decals")
	TArray<UMaterialInterface*> DecalMaterials;
	FTimerHandle TimerHandle;
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class FANTASYRPG_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	AProjectile();
	UPROPERTY(EditAnywhere, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;
	void FireInDirection(const FVector& ShootDirection);
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "PublicEnums.h"
#include "PrimitiveEnemy.generated.h"


UCLASS()
class FANTASYRPG_API APrimitiveEnemy : public AEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Montages")
	TObjectPtr<UAnimMontage> DeadAnimationMontage;
	UPROPERTY(EditAnywhere, Category = "Montages")
	TArray<FName> DeadAnimationSequence;
private:
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) override;
	virtual void ProcessDeath(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation);
	virtual void ProcessHit( bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation);
	virtual void LayingDead() override;
};



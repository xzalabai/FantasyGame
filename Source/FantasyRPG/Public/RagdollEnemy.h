// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "RagdollEnemy.generated.h"

UCLASS()
class FANTASYRPG_API ARagdollEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) override;
	virtual void ProcessDeath(bool bForwardHit, const FVector& ImpactPoint, const FVector& HitLocation) override;
	virtual void ProcessHit(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation) override;
};


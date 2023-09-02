// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "PrimitiveEnemy.generated.h"

UCLASS()
class FANTASYRPG_API APrimitiveEnemy : public AEnemy
{
	GENERATED_BODY()

private:
	virtual void OnReceivedHit(const FVector& ImpactPoint, int Damage) override;

private:
	// Montages
	UPROPERTY(EditAnywhere, Category=Montages)
	UAnimMontage* AnimMontage;
	UFUNCTION(BlueprintCallable)
	void LayingDead();
	virtual void ProcessDeath(bool bForwardHit);
	virtual void ProcessHit(bool bForwardHit);
};

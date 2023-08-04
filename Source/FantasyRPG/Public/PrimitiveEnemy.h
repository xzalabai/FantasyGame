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
	virtual void OnReceivedHit(const FVector& ImpactPoint) override;
	float DotProduct(const FVector& Vector1, const FVector& Vector2);

private:
	// Montages
	UPROPERTY(EditAnywhere, Category=Montages)
	UAnimMontage* DeathMontage;
	UFUNCTION(BlueprintCallable)
	void LayingDead();
};

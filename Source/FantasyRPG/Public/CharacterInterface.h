// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API ICharacterInterface
{
	GENERATED_BODY()
public:
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) = 0;
	virtual void OnPerfectBlockReceived() = 0;
	virtual void BlockAttack(const FVector& ImpactDirection, int Damage) = 0;
	UFUNCTION(BlueprintCallable)
	virtual void PerformActionOnNotify() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void AttackStart() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void ReloadEnd() = 0;

};

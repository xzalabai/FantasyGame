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
	// Called when received a hit
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) = 0;
	// Called when attacker perfectly blocked attack (slowmo)
	virtual void OnPerfectBlockReceived() = 0;
	// Method for Blocking attack
	virtual void BlockAttack(const FVector& ImpactDirection, int Damage) = 0;
	// Perform method when ActionOnNotify from Montage is called
	UFUNCTION(BlueprintCallable)
	virtual void PerformActionOnNotify() = 0;
	UFUNCTION(BlueprintCallable)
	// Called when Attack started
	virtual void AttackStart() = 0;
	UFUNCTION(BlueprintCallable)
	// Called when Attack ended
	virtual void AttackEnd() = 0;
	UFUNCTION(BlueprintCallable)
	// Called when Reloading a weapon is finished
	virtual void ReloadEnd() = 0;
	// Method when Controller (Player/Bot) wants to attack
	UFUNCTION(BlueprintCallable)
	virtual void InitiateAttack() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void LayingDead() = 0;

};

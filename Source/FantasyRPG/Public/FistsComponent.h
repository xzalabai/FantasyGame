// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponInterface.h"
#include "FistsComponent.generated.h"

class AHeroCharacter;
class AFist;

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UFistsComponent : public UActorComponent, public IWeaponInterface
{
	GENERATED_BODY()

public:	
	UFistsComponent();
	void RegisterHandColliders();
	void EnableOverlappingEvents(bool bEnable);
	UFUNCTION(BlueprintCallable)
	void PerformBoxTraceOnFists();
	virtual void InitiateAttack(class AHeroCharacter &Character, class UAnimInstance &AnimInstance);
	virtual void AttackMontageStarted() override;
	virtual void AttackMontageEnded() override;
protected:
	virtual void BeginPlay() override;
	// Animations properties -----------------------------
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	TArray<FName> AnimationSequenceName;
	// ----------------------------------------------------
private:
	UPROPERTY()
	AFist* LeftHand;
	UPROPERTY()
	AFist* RightHand;
	UPROPERTY()
	AFist* RightHandRef;
	UPROPERTY()
	AFist* LeftHandRef;
	AHeroCharacter* GetHero();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFist> FistBPClass;
};

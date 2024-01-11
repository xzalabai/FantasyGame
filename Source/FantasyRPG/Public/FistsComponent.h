// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipableInterface.h"
#include "FistsComponent.generated.h"

class AHeroCharacter;
class AFist;

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UFistsComponent : public UActorComponent, public IEquipableInterface
{
	GENERATED_BODY()

public:	
	UFistsComponent();
	void RegisterHandColliders();
	void EnableOverlappingEvents(bool bEnable);
	UFUNCTION(BlueprintCallable)
	void PerformBoxTrace();
	virtual void PerformMontage(class UAnimInstance* AnimInstance, FName MontageName = "", class UAnimMontage* AnimMontage = nullptr) override;
	virtual void AttackMontageStarted() override;
	virtual void AttackMontageEnded() override;
	virtual void PerformActionOnNotify() override;
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
	AHeroCharacter* GetHero();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFist> FistBPClass;
};

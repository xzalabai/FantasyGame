// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThrowableInterface.h"
#include "EquipableInterface.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class FANTASYRPG_API AGrenade : public AItem, public IThrowableInterface, public IEquipableInterface
{
	GENERATED_BODY()
	
public:	
	AGrenade();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Throw(FVector& Direction) override;
	UFUNCTION()
	virtual void OnThrowableOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void OnItemEquipped(AHeroCharacter *MainCharacter) override;
	void TogglePhysics(bool enable);
	void PerformSphereTrace();
	virtual void PerformMontage(class UAnimInstance* AnimInstance, FName MontageName = "", class UAnimMontage* AnimMontage = nullptr) override;
	virtual void EnableOverlappingEvents(bool Enable);
	virtual void AttackMontageStarted() override;
	virtual void AttackMontageEnded() override;
	virtual void PerformActionOnNotify() override;
	UPROPERTY(EditDefaultsOnly, Category = "Item Properties")
	float ImpactArea = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Item Properties")
	int Damage = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Item Properties")
	float ThrowForce = 1500.0f;
	UPROPERTY()
	AHeroCharacter* PreviousOwner = nullptr;
};






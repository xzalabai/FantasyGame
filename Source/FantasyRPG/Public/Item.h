// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PublicEnums.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "HeroCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Item.generated.h"


class UStaticMeshComponent;
class UCapsuleComponent;
class USphereComponent;
class UNiagaraComponent;
class AHeroCharacter;
class UAnimInstance;

UCLASS()
class FANTASYRPG_API AItem : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	AItem();
	UPROPERTY(EditAnywhere, Category = "Gameplay Tags")
	FGameplayTagContainer ItemTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere);
	UNiagaraComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere);
	USphereComponent* TriggerCollider;
	UPROPERTY(EditDefaultsOnly);
	EItemType ItemType;
	UPROPERTY(EditAnywhere);
	bool bAutoEquip = false;
	virtual void Equip();
	virtual void Unequip();
	void AttachToSocket(USkeletalMeshComponent* MeshComp, FName SocketName);
	FORCEINLINE bool IsAutoEquip() const { return bAutoEquip;}
	virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override { TagContainer = ItemTag; return;}
protected:
	UPROPERTY(EditInstanceOnly)	
	EItemState ItemState = EItemState::EIS_LayingOnGround;
	void PerformMontage(AHeroCharacter &Character, UAnimInstance &AnimInstance);
	// Animations properties -----------------------------
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	TArray<FName> AnimationSequenceName;
	AHeroCharacter* GetOwnerCharacter();
};

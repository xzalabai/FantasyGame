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
class UDAItem;

UCLASS()
class FANTASYRPG_API AItem : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Item Details")
	FGameplayTagContainer ItemTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere);
	UNiagaraComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere);
	USphereComponent* TriggerCollider;
	UPROPERTY(EditDefaultsOnly);
	EItemType ItemType;
	virtual void Equip();
	virtual void Unequip();
	void AttachToSocket(USkeletalMeshComponent* MeshComp, FName SocketName);
	virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override { TagContainer = ItemTag; return;}
	FORCEINLINE bool IsAutoEquip() const { return bAutoEquip; }
	FORCEINLINE bool IsAvailableToInventory() const { return bAvailableToInventory; }
	virtual void OnItemEquipped(AHeroCharacter &MainCharacter);

	// UDAItem Data ------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Item Properties for Inventory")
	UDAItem* DAItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties for Inventory")
	FString AssetName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties for Inventory")
	UTexture2D* AssetThumbnail;
	UPROPERTY(EditAnywhere, Category = "Item Properties for Inventory");
	bool bAvailableToInventory = false;
	UPROPERTY(EditAnywhere, Category = "Item Properties for Inventory");
	bool bAutoEquip = false;
	// --------------------------------------------
protected:
	UPROPERTY(EditInstanceOnly)	
	EItemState ItemState = EItemState::EIS_LayingOnGround;
	void PerformMontage(UAnimInstance *AnimInstance);
	void PerformMontage(UAnimInstance *AnimInstance,  FName& MontageName,  UAnimMontage* AnimMontage);
	// Animations properties -----------------------------
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, Category=AnimationProperties)
	TArray<FName> AnimationSequenceName;
	const AHeroCharacter* GetOwnerCharacter() const;
};




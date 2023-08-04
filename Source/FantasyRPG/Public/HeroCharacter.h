// Fill out your copyright notice in the Description page of Project Settings.
//
// TODO: refactor Fists and handling of Right/Left Hand colliders. reference should be assigned in the constructor of fistsComponent
// TODO: refactor Fist -> create cpp base
// TODO: HIGH! (linked with Weapon to IWeapon) change equippedWeapon to equippedItem (so we can swap grenade with weapon)
// TODO: LOW replace animation BP for rrunning with Item
// TODO: HIGH! create UActorComponent for weapon equiping
// TODO: HIGH! change Weapon to MeeleWeapon: public IWeaponInterface ..... FireWeapon: public IWeaponInterface 
// TODO: MED add to enemy HP
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PublicEnums.h"
#include "HeroCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class UAttributesComponent;
class UFistsComponent;
class UBoxComponent;
class UWeaponComponent;
class AFist;
class AProjectile;

UCLASS()
class FANTASYRPG_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroCharacter();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void AttackStart();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE UAttributesComponent* GetAttributes() const { return Attributes;}
	UPROPERTY(BlueprintReadWrite, Category=Weapon)
	AItem* EquippedWeapon = nullptr;
	AItem* EquippedItem = nullptr;

protected:
	// Input context
	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* InputContext;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* EquipAction;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void InitiateAttack();
	void ToggleEquip();
	void AutoEquip(AItem *Item);
	void InitiateAttackWithWeapon();
	void InitiateAttackWithItem();
	void InitiateAttackWithoutWeapon();
	void Equip(AItem* Item);	
	void Unequip();
	void SwapItem(AItem* WeaponToBeEquipped);


	UPROPERTY()
	AItem* OverlappedItem;
	UPROPERTY(EditAnywhere)
	UBoxComponent* LeftHandCollider;
	UPROPERTY(EditAnywhere)
	UBoxComponent* RightHandCollider;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UFistsComponent* Fists;
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UWeaponComponent* WeaponController;

	// Montages
	UPROPERTY(EditAnywhere, Category=Montages)
	UAnimMontage* MontageAttack;

	// Enums
	UPROPERTY(BlueprintReadOnly)	
	ECharacterState CharacterState = ECharacterState::ECS_WithoutWeapon;
	UPROPERTY(BlueprintReadOnly)		
	EAnimationState AnimationState = EAnimationState::EAS_NoAnimation;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void AttachItemToSocket(AItem* Item, FName SocketName); 
	// shooting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;
private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;
	void FireFromGun();
};








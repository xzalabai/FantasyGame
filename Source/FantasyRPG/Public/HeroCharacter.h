// TODO: refactor Fists and handling of Right/Left Hand colliders. reference should be assigned in the constructor of fistsComponent

// TODO: HIGH! create UActorComponent for weapon equiping
// TODO: HIGH! add rotation (rotate 3rd person character with mouse unreal)
// TODO: HIGH! Fix Swap() weapons
// TODO: HIGH! Change PrimitiveEnemy to RootBone enemy
// TODO: HIGH! Fix Firing from weapon while running (there is no animation so there is no triggered AttackStart and AttackEnd)
// TODO: HIGH! Use const for functions and parameters
// TODO: HIGH! Use const for GETTERS IN GETOWNERCHARACTER AND GETPLAYERCAMERA IN FireWeapon!!
// TODO: MED add to enemy HP
// TODO: MED change animation while carying a melee weapon
// TODO: MED unify naming for input handlers (Reload, Release...)
// TODO: MED add PerformOnRelease to IEquipable
// TODO: MED unify naming (PerformOn, InitiateAttack,...)
// TODO: MED fix error when you Equip and Enemy is near
// TODO: MED remove ECharacterState and replace it in animation with gameplay tag (now it's used only for ABP)
// TODO: LOW replace animation BP for rrunning with Item
// TODO: LOW cache AHeroCharacter into the FireWeapon and reuse Unequip (set it to nullptr)
// TODO: fix issue while shooting -> click (timer is active), then hold (we don't get any input to try to shoot again)
// --------------------------------------------------------------
// Features:
// - dynamic crosshair based on aim / move
// - AI
// - correct animations
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
class AFist;
class AProjectile;
class UGameplayTagsManager;
class UCameraComponent;
class UInventoryComponent;
class UDAItem;

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
	UFUNCTION(BlueprintCallable)
	void ReloadEnd();
	UFUNCTION(BlueprintCallable)
	void PerformActionOnNotify();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE UAttributesComponent* GetAttributes() const { return Attributes;}
	FORCEINLINE bool IsAiming() const { return bIsAiming;}
	UPROPERTY(BlueprintReadWrite, Category=Item)
	mutable AItem* EquippedItem = nullptr;
	bool CharacterIsMoving() const;
	const UCameraComponent* GetCharacterCamera() const;
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponFired() const;
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
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* ReleaseAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void InitiateAttack();
	void ToggleEquip();
	void Reload();
	void MouseRelease();
	void AutoEquip(AItem *Item);
	void Equip(AItem* Item);	
	void Unequip();
	void SwapItem(AItem* ItemToBeEquipped);
	
	UPROPERTY()
	AItem* OverlappedItem;
	UPROPERTY(EditAnywhere)
	UBoxComponent* LeftHandCollider;
	UPROPERTY(EditAnywhere)
	UBoxComponent* RightHandCollider;
	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UFistsComponent* Fists;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAiming = false;
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent>  ItemInventory;
	UFUNCTION(BlueprintNativeEvent)
	void InventoryItemsUpdated();
private:		
	UFUNCTION()
	bool HasItemTag(const AItem *Item, const FName TagName) const;
	FORCEINLINE UObject* GetEquippeddItem();
	void InsertToInventory(AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(UDAItem* DAItem);

};








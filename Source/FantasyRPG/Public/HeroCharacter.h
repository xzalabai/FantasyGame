// TODO: refactor Fists and handling of Right/Left Hand colliders. reference should be assigned in the constructor of fistsComponent

// TODO: HIGH! create UActorComponent for weapon equiping
// TODO: HIGH! Find out a way how to fire a weapon (now character is standing)
// TODO: HIGH! add rotation (rotate 3rd person character with mouse unreal)
// TODO: HIGH! Fix Swap() weapons
// TODO: HIGH! Fix Firing from weapon while running (there is no animation so there is no triggered AttackStart and AttackEnd)
// TODO: HIGH! Use const for functions and parameters
// TODO: MED add to enemy HP
// TODO: MED unify naming for input handlers (Reload, Release...)
// TODO: MED add PerformOnRelease to IEquipable
// TODO: MED unify naming (PerformOn, InitiateAttack,...)
// TODO: MED remove ECharacterState and replace it in animation with gameplay tag (now it's used only for ABP)
// TODO: LOW replace animation BP for rrunning with Item
// TODO: LOW cache AHeroCharacter into the FireWeapon and reuse Unequip (set it to nullptr)
// TODO: fix issue while shooting -> click (timer is active), then hold (we don't get any input to try to shoot again)
// --------------------------------------------------------------
// Features:
// - Aiming based on the camera aim
// - Inventory
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
	UPROPERTY(BlueprintReadWrite, Category=Item)
	AItem* EquippedItem = nullptr;
	bool CharacterIsMoving() const;
	UCameraComponent* GetCharacterCamera();

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
	void InitiateAttackWithItem();
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
private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;
	UFUNCTION()
	bool HasItemTag(const AItem *Item, const FName TagName) const;
	FORCEINLINE UObject* GetEquippeddItem();


};








// TODO: refactor Fists and handling of Right/Left Hand colliders. reference should be assigned in the constructor of fistsComponent
// TODO: HIGH! create UActorComponent for weapon equiping
// TODO: HIGH! Fix Swap() weapons
// TODO: HIGH! replace nullptr with Attacker in all OnReceivedHit
// TODO: HIGH! Replace all editable values (e.g. for Weapon -> Asset Name, Fire Rate, Magazine, etc) with DataTable
// TODO: HIGH! Add DataTable for Event WeaponFired in Blueprint (so it picks up correct recoil).
// TODO: HIGH! Separate logic in HeroCharacter
// TODO: HIGH! Cache Character and replace GetOwnerCharacter 
// TODO: HIGH! Figure out better way of grenade -> USceneComponent is not moving during Mesh->AddImpulse, so we have to get Mesh location for SPhere trace
// TODO: HIGH! use some Interface instead of casting to class
// TODO: HIGH! Fix Reloading (running during reload) - edge case - reload interrupted
// TODO: HIGH! Use const for functions and parameters
// TODO: MED find out if you can add CONST to Attacker in OnReceivedHit
// TODO: MED change animation while carying a melee weapon
// TODO: MED unify naming for input handlers (Reload, Release...)
// TODO: MED Change logic -> let enemies subscribe to HeroCharacter
// TODO: MED unify naming (PerformOn, InitiateAttack,...)
// TODO: MED fix error when you Equip and Enemy is near
// TODO: LOW replace animation BP for rrunning with Item
// TODO: LOW cache AHeroCharacter into the FireWeapon and reuse Unequip (set it to nullptr)
// TODO: fix issue while shooting -> click (timer is active), then hold (we don't get any input to try to shoot again)
// --------------------------------------------------------------
// Features:
// - Stealth
// - Inventory with Data tables (for weapons)
// - cover behind wall
// - AI
// - correct animations
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterInterface.h"
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
class FANTASYRPG_API AHeroCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AHeroCharacter();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void MontageEnd();
	UFUNCTION(BlueprintCallable)
	void AttackStart();
	UFUNCTION(BlueprintCallable)
	void BlockEnd();
	UFUNCTION(BlueprintCallable)
	void ReloadEnd();
	UFUNCTION(BlueprintCallable)
	void PerformActionOnNotify();
	void OnPerfectBlockReceived() override;
	void BlockAttack(const FVector& ImpactDirection, int Damage) override;
	void InitiateBlock();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) override;
	bool IsRotatedTowardsAttacker(const TObjectPtr<AActor> Attacker) const;
	FORCEINLINE UAttributesComponent* GetAttributes() const { return Attributes;}
	FORCEINLINE bool IsAiming() const { return bIsAiming;}
	FORCEINLINE bool IsBlocking() const { return bIsBlocking;}
	FORCEINLINE bool IsPerfectBlocking() const { return (!bIsBlockingBeforeAttack && bIsBlocking);}
	UPROPERTY(BlueprintReadWrite, Category=Item)
	mutable AItem* EquippedItem = nullptr;
	bool CharacterIsMoving() const;
	const UCameraComponent* GetCharacterCamera() const;
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponFired(const FString& AssetName) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAimSpread() const;
	float GetCharacterPitch() const;
	UObject* GetEquippedItem();
protected:
	// Input context -----------------------------------
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
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BlockAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InsertAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;
	// ------------------------------------------------

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void InitiateAttack();
	void ToggleEquip();
	void Insert();
	void Reload();
	void MouseRelease();
	void AbsorbItem(AItem* Item);
	void Equip(AItem* Item);
	void Unequip();
	void InitiateDodge();
	void SwapItem(AItem* ItemToBeEquipped);
	// ---------------------------------------------------
	
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
	
	// Montages----------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Animation properties")
	UAnimMontage* HitReactionMontage;
	UPROPERTY(EditAnywhere, Category = "Animation properties")
	UAnimMontage* DodgeMontage;
	// -------------------------------------------------------
	
	// Enums--------------------------------------------------
	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType = EItemType::EIT_NoItem;
	UPROPERTY(BlueprintReadOnly)		
	EAnimationState AnimationState = EAnimationState::EAS_NoAnimation;
	// -------------------------------------------------------
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsBlocking = false;
	bool bIsBlockingBeforeAttack = false;
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent>  ItemInventory;
	UFUNCTION(BlueprintNativeEvent)
	void InventoryItemsUpdated();
	UFUNCTION(BlueprintNativeEvent)
	void AttackBlocked();
	UFUNCTION(BlueprintNativeEvent)
	void PerfectAttackBlocked();
private:		
	UFUNCTION()
	bool HasItemTag(const AItem *Item, const FName TagName) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasMeeleWeapon() const;
	void InsertToInventory(AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(UDAItem* DAItem);
	UPROPERTY(EditAnywhere, Category = "AnimationProperties")
	TArray<FName> HitReactionAnimationSequence;
	void PerformPerfectBlockReaction(AActor* Attacker);
	void EnemyAttackStarted();
};








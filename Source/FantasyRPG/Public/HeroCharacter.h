// TODO: refactor Fists and handling of Right/Left Hand colliders. reference should be assigned in the constructor of fistsComponent
// TODO: HIGH! create UActorComponent for weapon equiping
// TODO: HIGH! Fix Swap() weapons
// TODO: HIGH! replace nullptr with Attacker in all OnReceivedHit
// TODO: low! Add DataTable for Event WeaponFired in Blueprint (so it picks up correct recoil).
// TODO: HIGH! Separate logic in HeroCharacter
// TODO: HIGH! Change calling InitiateAttack_BP which calls BP (and plays montage) to C++ solution -> find good alternative to OnCompleted montage
// TODO: HIGH! Cache Character and replace GetOwnerCharacter 
// TODO: HIGH! Figure out better way of grenade -> USceneComponent is not moving during Mesh->AddImpulse, so we have to get Mesh location for SPhere trace
// TODO: HIGH! use some Interface instead of casting to class
// TODO: HIGH! Fix Reloading (running during reload) - edge case - reload interrupted
// TODO: HIGH! Use const for functions and parameters
// TODO: MED refactor animation play for enemies in method
// TODO: MED find out if you can add CONST to Attacker in OnReceivedHit
// TODO: MED change animation while carying a melee weapon
// TODO: MED unify naming for input handlers (Reload, Release...)
// TODO: MED unify naming (PerformOn, InitiateAttack,...)
// TODO: MED fix error when you Equip and Enemy is near
// TODO: LOW replace animation BP for rrunning with Item
// TODO: LOW cache AHeroCharacter into the FireWeapon and reuse Unequip (set it to nullptr)
// TODO: fix issue while shooting -> click (timer is active), then hold (we don't get any input to try to shoot again)
// --------------------------------------------------------------
// Features:
// - cover behind wall
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
	void SpecialAttackStart();
	UFUNCTION(BlueprintCallable)
	void SpecialAttackEnd();
	UFUNCTION(BlueprintCallable)
	void PerformActionOnNotify();
	void OnPerfectBlockReceived() override;
	void BlockAttack(const FVector& ImpactDirection, int Damage) override;
	void InitiateBlock();
	virtual void LayingDead() override {};

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) override;
	bool IsRotatedTowardsAttacker(const TObjectPtr<AActor> Attacker) const;
	FORCEINLINE UAttributesComponent* GetAttributes() const { return Attributes;}
	FORCEINLINE bool IsAiming() const { return bIsAiming;}
	FORCEINLINE bool IsBlocking() const { return bIsBlocking;}
	FORCEINLINE bool IsPerfectBlocking() const { return bIsPerfectBlocking;}
	UPROPERTY(BlueprintReadWrite, Category=Item)
	mutable AItem* EquippedItem = nullptr;
	bool CharacterIsMoving() const;
	const UCameraComponent* GetCharacterCamera() const;
	UFUNCTION(BlueprintImplementableEvent)
	void WeaponFired(const FName& NameID) const;
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
	virtual void InitiateAttack() override;
	virtual void InitiateAttack(const FName AttackName);
	void ToggleEquip();
	void Insert();
	void Reload();
	void MouseRelease();
	void AbsorbItem(AItem* Item);
	void Equip(AItem* Item);
	void Unequip();
	void InitiateDodge(const FInputActionValue& Value);
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
	UPROPERTY(EditAnywhere, Category = "Animation properties")
	TMap<FName, FName> DodgeMontageSequenceNames;
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
	bool bIsImmortal = false;
	bool bPerfectBlockTimePeriod = false;
	bool bIsPerfectBlocking = false;
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UInventoryComponent>  ItemInventory;
	UFUNCTION(BlueprintNativeEvent)
	void InventoryItemsUpdated();
	UFUNCTION(BlueprintNativeEvent)	
	void AttackBlocked();
private:		
	UFUNCTION()
	bool HasItemTag(const AItem *Item, const FName TagName) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasWeapon(const FName WeaponTag = "") const;
	FORCEINLINE bool IsImmortal() const { return bIsImmortal; };
	void InsertToInventory(AItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(UDAItem* DAItem);
	UPROPERTY(EditAnywhere, Category = "AnimationProperties")
	TArray<FName> HitReactionAnimationSequence;
	void PerformPerfectBlockReaction(AActor* Attacker);
	void OnEnemyAttack(bool bStart, int8 enemyID);
	FName ResolveDirection(const FVector2D& Input);
	friend class AEnemy;
	
};








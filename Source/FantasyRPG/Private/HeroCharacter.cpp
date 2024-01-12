#include "HeroCharacter.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Item.h"
#include "Animation/AnimMontage.h"
#include "Enemy.h"
#include "Math/UnrealMathUtility.h"
#include "Weapon.h"
#include "FireWeapon.h"
#include "DAItem.h"
#include "ThrowableInterface.h"
#include "Containers/UnrealString.h"
#include "AttributesComponent.h"
#include "Projectile.h"
#include "FistsComponent.h"
#include "InventoryComponent.h"
#include "Fist.h"
#include "Kismet/GameplayStatics.h"
#include "FireWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagsManager.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <FantasyRPG\FantasyRPGGameModeBase.h>

AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	ItemInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Fists = CreateDefaultSubobject<UFistsComponent>(TEXT("Fists"));
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext,0);
		}
	}
	if (UCapsuleComponent* Caps = GetCapsuleComponent())
	{
		Caps->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharacter::OnTriggerBeginOverlap);
		Caps->OnComponentEndOverlap.AddDynamic(this, &AHeroCharacter::OnTriggerEndOverlap);
	}
	CameraComponent = FindComponentByClass<UCameraComponent>();
	Fists->RegisterHandColliders();
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Jump);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Reload);
		EnhancedInputComponent->BindAction(ReleaseAction, ETriggerEvent::Triggered, this, &AHeroCharacter::MouseRelease);
		EnhancedInputComponent->BindAction(InsertAction, ETriggerEvent::Triggered, this, &AHeroCharacter::Insert);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHeroCharacter::InitiateAttack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AHeroCharacter::ToggleEquip);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AHeroCharacter::InitiateBlock);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AHeroCharacter::InitiateDodge);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AHeroCharacter::BlockEnd);
	}
}

void AHeroCharacter::OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage)
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] OnReceivedHit, IsBlocking() %d, IsPerfectBlocking() %d, IsImmortal() %d"), IsBlocking(), IsPerfectBlocking(), IsImmortal());
	
	if (IsImmortal())
	{
		return;
	}

	if (IsBlocking() && HasMeeleWeapon() && IsRotatedTowardsAttacker(Attacker))
	{
		if (IsPerfectBlocking())
		{
			PerformPerfectBlockReaction(Attacker);
		}
		else
		{
			// Block effect reaction
			AttackBlocked();
		}
	}
	else
	{
		Attributes->DecreaseHealth(Damage);
		PlayAnimMontage(HitReactionMontage, 1.0f, HitReactionAnimationSequence[FMath::RandRange(0, HitReactionAnimationSequence.Num() - 1)]);
		AnimationState = EAnimationState::EAS_AnimationInProgress;
	}
}

float AHeroCharacter::GetAimSpread() const
{
	float Velocity = UKismetMathLibrary::VSize(GetVelocity()) + (IsAiming() ? 0 : 50);
	return Velocity;
}

void AHeroCharacter::OnEnemyAttack(bool bStart, int8 enemyID)
{
	if (!bIsBlocking)
	{
		bPerfectBlockTimePeriod = bStart;
	}
	
	if (bStart)
		UE_LOG(LogTemp, Warning, TEXT("[HeroCharacter] EnemyAttackStart"));
	if (!bStart)
		UE_LOG(LogTemp, Warning, TEXT("[HeroCharacter] EnemyAttackEnd"));

	UE_LOG(LogTemp, Warning, TEXT("[HeroCharacter] EnemyAttack, HeroCharacter is blocking %d"), bIsBlocking ? 1 : 0);
}

void AHeroCharacter::PerformPerfectBlockReaction(AActor* Attacker)
{
	const FRotator RotationTowardsAttacker = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	SetActorRotation(RotationTowardsAttacker, ETeleportType::None);

	InitiateAttack("Attack3");
	
	if (ICharacterInterface* AttackerInterface = Cast<ICharacterInterface>(Attacker))
	{
		AttackerInterface->OnPerfectBlockReceived();
	}

	if (AFantasyRPGGameModeBase* GameMode = Cast<AFantasyRPGGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->WarpTime(0.3f);
	}
}

void AHeroCharacter::Move(const FInputActionValue& Value)
{
	if (IsBlocking())
	{
		return;
	}
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, MovementVector.Y); 
	AddMovementInput(RightDirection, MovementVector.X);
}

void AHeroCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
	const float CharacterPitch = GetCharacterPitch();
	
}

void AHeroCharacter::Jump()
{
	Super::Jump();
}


void AHeroCharacter::Insert()
{
	if (OverlappedItem && OverlappedItem->IsAvailableToInventory())
	{
		InsertToInventory(OverlappedItem);
	}
}

void AHeroCharacter::Reload()
{
	if (AFireWeapon* FireWeapon = Cast<AFireWeapon>(EquippedItem))
	{
		FireWeapon->ReloadWeapon();
		AnimationState = EAnimationState::EAS_AnimationInProgress;
	}
}

void AHeroCharacter::InitiateBlock()
{
	if (CharacterIsMoving())
	{
		return;
	}

	bIsBlocking = true;
	bIsPerfectBlocking = bPerfectBlockTimePeriod;
}

void AHeroCharacter::BlockEnd()
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] BlockEnd"));
	bIsBlocking = false;
	bIsPerfectBlocking = false;
}

void AHeroCharacter::BlockAttack(const FVector& ImpactDirection, int Damage)
{
	
}

void AHeroCharacter::RemoveFromInventory(UDAItem* DAItem)
{
	if (ItemInventory->RemoveFromInventory(DAItem))
	{
		InventoryItemsUpdated();
	}
}

void AHeroCharacter::MouseRelease()
{
	if (AFireWeapon* FireWeapon = Cast<AFireWeapon>(EquippedItem))
	{
		FireWeapon->OnMouseRelease();
		MontageEnd();
	}
}

void AHeroCharacter::ToggleEquip()
{
	if (AnimationState == EAnimationState::EAS_AnimationInProgress)
	{
		return;
	}

	bool bItemIsEquippable = Cast<IEquipableInterface>(OverlappedItem) != nullptr;

	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] ToggleEquip"));

	if (EquippedItem && !OverlappedItem)
	{
		Unequip();
	}
	else if (bItemIsEquippable && EquippedItem && OverlappedItem)
	{
		SwapItem(OverlappedItem);
	}
	else if (bItemIsEquippable && OverlappedItem && !EquippedItem)
	{
		Equip(OverlappedItem);			
	}
	else if (!bItemIsEquippable && OverlappedItem)
	{
		AbsorbItem(OverlappedItem);
	}
}


void AHeroCharacter::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AItem* Item = Cast<AItem>(OtherActor);
	if (!Item || Item->IsCurrentlyEquipped())
	{
		return;
	}
	OverlappedItem = Item;

	if (Item->IsAutoEquip())
	{
		AbsorbItem(OverlappedItem);
	}

	// TODO: show UI
}

void AHeroCharacter::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Unassign overlapped item that is marked as overlapping
	AItem* Item = Cast<AItem>(OtherActor);
	if (OverlappedItem == Item)
	{
		OverlappedItem = nullptr;
	}
}

void AHeroCharacter::InitiateAttack(const FName AttackName)
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] InitiateAttack, Animation state: %d"), AnimationState);
	if (AnimationState != EAnimationState::EAS_NoAnimation)
	{
		// Other animation in progress
		return;
	}

	AnimationState = EAnimationState::EAS_AnimationInProgress;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->PerformMontage(AnimInstance, AttackName);
}

void AHeroCharacter::InitiateAttack()
{	
	InitiateAttack("");
}

void AHeroCharacter::InitiateDodge(const FInputActionValue& Value)
{
	if (AnimationState != EAnimationState::EAS_NoAnimation)
	{
		// Other animation in progress
		return;
	}

	// Dodge
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	PlayAnimMontage(DodgeMontage, 1.0f, ResolveDirection(Value.Get<FVector2D>()));
	AnimationState = EAnimationState::EAS_AnimationInProgress;
}

void AHeroCharacter::AttackStart()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackStart"));
	AnimationState = EAnimationState::EAS_AnimationInProgress;
	IEquipableInterface *Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->AttackMontageStarted();
}

void AHeroCharacter::MontageEnd()
{
	// Called from ABP or script
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] MontageEnd"));
	AnimationState = EAnimationState::EAS_NoAnimation;
}

void AHeroCharacter::AttackEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackEnd"));
	MontageEnd();
	IEquipableInterface *Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->AttackMontageEnded();
}

void AHeroCharacter::SpecialAttackStart()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] SpecialAttackStart"));
	bIsImmortal = true;
}

void AHeroCharacter::SpecialAttackEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] SpecialAttackEnd"));
	if (AFantasyRPGGameModeBase* GameMode = Cast<AFantasyRPGGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->WarpTime(1.0f);
	}
	bIsImmortal = false;

}

void AHeroCharacter::ReloadEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] ReloadEnd"));
	MontageEnd();
}

void AHeroCharacter::PerformActionOnNotify()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] PerformActionOnNotify"));
	IEquipableInterface *Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->PerformActionOnNotify();
}

void AHeroCharacter::OnPerfectBlockReceived()
{
	return;
}

void AHeroCharacter::AttachItemToSocket(AItem* Item, FName SocketName)
{
	Item->AttachToSocket(GetMesh(), SocketName);
}

void AHeroCharacter::AbsorbItem(AItem *Item)
{
	Item->OnItemEquipped(this);
	if (EquippedItem)
	{
		EquippedItem->UpdateItemWithAttributes();
	}
	
}

void AHeroCharacter::SwapItem(AItem* ItemToBeEquipped)
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] SwapItem"));
	Unequip();
	Equip(ItemToBeEquipped);
}

void AHeroCharacter::Unequip()
{
	UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Unequip: %s"), *EquippedItem->GetName());
	ItemType = EItemType::EIT_NoItem;
	EquippedItem->OnItemUnequipped();
	EquippedItem = nullptr;
}

void AHeroCharacter::InsertToInventory(AItem* Item)
{
	if (ItemInventory->InsertToInventory(Item))
	{
		Item->Destroy();
	}
}

void AHeroCharacter::Equip(AItem* Item)
{
	if (HasItemTag(Item, FName("Weapon.MeeleWeapon")))
	{
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Equip(): Equipping a meele weapon: %s"), *Item->GetName());
	}
	else if (HasItemTag(Item, FName("Weapon.FireWeapon")))
	{
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Equip(): Equipping a fire weapon: %s"), *Item->GetName());
	}
	else if (HasItemTag(Item, FName("Item.Throwable")))
	{
		UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] Equip(): Equipping an item"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] Equip(): Picking unidentified object"));
		return;
	}
	AttachItemToSocket(Item, "RightHandSocket");
	OverlappedItem = nullptr;
	ItemType = Item->GetItemType();
	Item->OnItemEquipped(this);	
	EquippedItem = Item;
}

bool AHeroCharacter::CharacterIsMoving() const
{
	if (GetMovementComponent()->Velocity == FVector::ZeroVector)
		return false;
	return true;
}

bool AHeroCharacter::HasItemTag(const AItem *Item, const FName TagName) const
{
	if (!Item)
	{
		return false;
	}
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	return Item->ItemTag.HasTag(FGameplayTag::RequestGameplayTag(TagName));
}

void AHeroCharacter::InventoryItemsUpdated_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] InventoryItemsUpdated should be overriden in class blueprint!"));
}

void AHeroCharacter::AttackBlocked_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] AttackBlocked should be overriden in class blueprint!"));
}
	
const UCameraComponent* AHeroCharacter::GetCharacterCamera() const
{
	return CameraComponent;
}

UObject* AHeroCharacter::GetEquippedItem()
{
	// returns Fists or Currently equipped weapon
	if (!EquippedItem)
	{
		return Fists;
	}
	return EquippedItem;
}

bool AHeroCharacter::HasMeeleWeapon() const
{
	return HasItemTag(EquippedItem, FName("Weapon.MeeleWeapon"));
}

float AHeroCharacter::GetCharacterPitch() const
{
	float Pitch = Controller->GetControlRotation().Pitch;
	return Pitch > 270 ? ((Pitch - 360)) : Pitch;
}

bool AHeroCharacter::IsRotatedTowardsAttacker(const TObjectPtr<AActor> Attacker) const
{
	float DotProduct = FVector::DotProduct(Attacker->GetActorForwardVector(), GetActorForwardVector());
	return DotProduct <= 0;
}

FName AHeroCharacter::ResolveDirection(const FVector2D& Input) {
	if (Input.X == 0 && Input.Y == 1) {
		return DodgeMontageSequenceNames["Forward"];
	}
	else if (Input.X == 0 && Input.Y == -1) {
		return DodgeMontageSequenceNames["Backwards"];
	}
	else if (Input.X == 1 && Input.Y == 0) {
		return DodgeMontageSequenceNames["Right"];
	}
	else if (Input.X == -1 && Input.Y == 0) {
		return DodgeMontageSequenceNames["Left"];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] ResolveDirection has unresolved key mapping!"));
		return "";
	}
	
}
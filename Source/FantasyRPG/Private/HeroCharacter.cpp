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

	// Filter enemies and subscribe to them
	TArray<AActor*> EnemiesFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemiesFound);
	for (AActor* EnemyActor : EnemiesFound)
	{
		if (AEnemy* Enemy = Cast<AEnemy>(EnemyActor))
		{
			Enemy->OnEnemyAttackStarted.AddUObject(this, &AHeroCharacter::EnemyAttackStarted);
		}
	}
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHeroCharacter::InitiateAttack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AHeroCharacter::ToggleEquip);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AHeroCharacter::InitiateBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AHeroCharacter::BlockEnd);
	}
}

void AHeroCharacter::OnReceivedHit(const FVector& ImpactDirection, AActor* Attacker, int Damage)
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] OnReceivedHit, IsBlocking() %d, IsPerfectBlocking() %d"), IsBlocking(), IsPerfectBlocking());
	if (IsBlocking() && HasMeeleWeapon())
	{
		if (IsPerfectBlocking())
		{
			PerformPerfectBlockReaction(Attacker);
		}
		else
		{
			// If he stands in front of us (dot product)
			// Block effect reaction
			AttackBlocked();
		}
	}
	else
	{
		Attributes->DecreaseHealth(Damage);
		PlayAnimMontage(HitReactionMontage, 1.0f, HitReactionAnimationSequence[FMath::RandRange(0, HitReactionAnimationSequence.Num() - 1)]);
	}
}

void AHeroCharacter::EnemyAttackStarted()
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] EnemyAttackStarted, HeroCharacter is blocking %d"), bIsBlocking ? 1 : 0);
	bIsBlockingBeforeAttack = IsBlocking() ? true : false;
}

void AHeroCharacter::PerformPerfectBlockReaction(AActor* Attacker)
{
	const FName BlockReactionMontageName = FName(TEXT("PerfectBlock1"));
	const FRotator RotationTowardsAttacker = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	
	SetActorRotation(RotationTowardsAttacker, ETeleportType::None);
	InitiateAttack();
	if (ICharacterInterface* AttackerInterface = Cast<ICharacterInterface>(Attacker))
	{
		AttackerInterface->OnPerfectBlockReceived();
	}
	// Give a chance to BP
	PerfectAttackBlocked();
	bIsBlockingBeforeAttack = false;
}

void AHeroCharacter::Move(const FInputActionValue& Value)
{
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
}

void AHeroCharacter::Jump()
{
	Super::Jump();
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
	BlockStart();
}

void AHeroCharacter::BlockStart()
{
	bIsBlocking = true;
}

void AHeroCharacter::BlockEnd()
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] BlockEnd"));
	bIsBlocking = false;
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
		AnimationState = EAnimationState::EAS_NoAnimation;
	}
}

void AHeroCharacter::ToggleEquip()
{
	if (OverlappedItem && OverlappedItem->IsAvailableToInventory())
	{
		InsertToInventory(OverlappedItem);
	}
	else if (EquippedItem && !OverlappedItem)
	{
		Unequip();
	}
	else if (EquippedItem && OverlappedItem)
	{
		SwapItem(OverlappedItem);
	}
	else if (OverlappedItem && !EquippedItem)
	{
		Equip(OverlappedItem);			
	}
}


void AHeroCharacter::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AItem* Item = Cast<AItem>(OtherActor);
	if (!Item)
	{
		return;
	}
	OverlappedItem = Item;

	if (Item->IsAutoEquip())
	{
		AutoEquip(OverlappedItem);
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

void AHeroCharacter::InitiateAttack()
{	

	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] InitiateAttack, Animation state: %d"), AnimationState);
	if (AnimationState != EAnimationState::EAS_NoAnimation)
	{
		// Other animation in progress
		return;
	}

	AnimationState = EAnimationState::EAS_AnimationInProgress;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	IEquipableInterface *Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->PerformMontage(AnimInstance);
}

void AHeroCharacter::AttackStart()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackStart"));
	AnimationState = EAnimationState::EAS_AnimationInProgress;
	IEquipableInterface *Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->AttackMontageStarted();
}

void AHeroCharacter::AttackEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackEnd"));
	AnimationState = EAnimationState::EAS_NoAnimation;
	IEquipableInterface *Item = Cast<IEquipableInterface>(GetEquippedItem());
	Item->AttackMontageEnded();
}

void AHeroCharacter::ReloadEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] ReloadEnd"));
	AnimationState = EAnimationState::EAS_NoAnimation;
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

void AHeroCharacter::AutoEquip(AItem *Item)
{
	Item->OnItemEquipped(*this);
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
	CharacterState = ECharacterState::ECS_WithoutWeapon;
	EquippedItem->Unequip();
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
	AttachItemToSocket(Item, "RightHandSocket");
	OverlappedItem = nullptr;
	if (HasItemTag(Item, FName("Weapon.MeeleWeapon")))
	{
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Equip(): Equipping a meele weapon: %s"), *Item->GetName());
		CharacterState = ECharacterState::ECS_WithMeeleWeapon;
	}
	else if (HasItemTag(Item, FName("Weapon.FireWeapon")))
	{
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Equip(): Equipping a fire weapon: %s"), *Item->GetName());
		CharacterState = ECharacterState::ECS_WithFireWeapon;
	}
	else if (HasItemTag(Item, FName("Item.Throwable")))
	{
		UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] Equip(): Equipping an item"));
		CharacterState = ECharacterState::ECS_WithItem;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] Equip(): Picking unidentified object"));
		return;
	}
	Item->OnItemEquipped(*this);	
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

void AHeroCharacter::PerfectAttackBlocked_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] PerfectAttackBlocked should be overriden in class blueprint!"));
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

bool AHeroCharacter::HasMeeleWeapon()
{
	return HasItemTag(EquippedItem, FName("Weapon.MeeleWeapon"));
}
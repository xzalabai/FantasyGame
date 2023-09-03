#include "HeroCharacter.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Item.h"
#include "Animation/AnimMontage.h"
#include "Math/UnrealMathUtility.h"
#include "Weapon.h"
#include "FireWeapon.h"
#include "PickupInterface.h"
#include "ThrowableInterface.h"
#include "Containers/UnrealString.h"
#include "AttributesComponent.h"
#include "Projectile.h"
#include "FistsComponent.h"
#include "Fist.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagsManager.h"
#include "GameFramework/PawnMovementComponent.h"

AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
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
	if (!CameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] Camera not found!"));
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHeroCharacter::InitiateAttack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AHeroCharacter::ToggleEquip);
	}
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

void AHeroCharacter::ToggleEquip()
{
	if (EquippedItem && !OverlappedItem)
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
	// Invoked from Binding
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] InitiateAttack, Animation state: %d"), AnimationState);
	if (AnimationState != EAnimationState::EAS_NoAnimation)
	{
		// Other animation in progress
		return;
	}

	AnimationState = EAnimationState::EAS_AnimationInProgress;
	
	IWeaponInterface *Weapon = Cast<IWeaponInterface>(EquippedItem);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!Weapon)
	{
		// We don't have a weapon, so use fists
		Weapon = Cast<IWeaponInterface>(Fists);
	}
	Weapon->PerformMontage(*this, *AnimInstance);
}

void AHeroCharacter::AttackStart()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackStart"));
	AnimationState = EAnimationState::EAS_AnimationInProgress;
	if (HasItemTag(EquippedItem, "Weapon.Throwable"))
	{
		return;
	}
	IWeaponInterface *Weapon = Cast<IWeaponInterface>(EquippedItem);
	if (!Weapon)
	{
		Weapon = Cast<IWeaponInterface>(Fists);
	}
	Weapon->AttackMontageStarted();
}

void AHeroCharacter::AttackEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackEnd"));
	AnimationState = EAnimationState::EAS_NoAnimation;
	if (HasItemTag(EquippedItem, "Weapon.Throwable"))
	{
		if (IThrowableInterface* Throwable = Cast<IThrowableInterface>(EquippedItem))
		{
			FVector Direction = GetActorForwardVector();
			Throwable->Throw(Direction);
		}
		if (IWeaponInterface *Weapon = Cast<IWeaponInterface>(EquippedItem))
		{
			Weapon->AttackMontageEnded();
		}
		return;
	}
	IWeaponInterface *Weapon = Cast<IWeaponInterface>(EquippedItem);
	if (!Weapon)
	{
		Weapon = Cast<IWeaponInterface>(Fists);
	}
	Weapon->AttackMontageEnded();

}

void AHeroCharacter::AttachItemToSocket(AItem* Item, FName SocketName)
{
	Item->AttachToSocket(GetMesh(), SocketName);
}

void AHeroCharacter::AutoEquip(AItem *Item)
{
	if (IPickupInterface* PickabbleItem = Cast<IPickupInterface>(Item))
	{
		PickabbleItem->OnItemEquipped(*this);
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
	CharacterState = ECharacterState::ECS_WithoutWeapon;
	EquippedItem->Unequip();
	EquippedItem = nullptr;
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
	else if (HasItemTag(Item, FName("Weapon.Throwable")))
	{
		UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] Equip(): Equipping an item"));
		IPickupInterface* PickableItem = Cast<IPickupInterface>(Item);
		PickableItem->OnItemEquipped(*this);
		CharacterState = ECharacterState::ECS_WithItem;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] Equip(): Picking unidentified object"));
		return;
	}	
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

UCameraComponent* AHeroCharacter::GetCharacterCamera()
{
	return CameraComponent;
}

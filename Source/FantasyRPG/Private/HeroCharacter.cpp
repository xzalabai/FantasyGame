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
#include "WeaponComponent.h"
#include "Fist.h"
#include "GameFramework/PawnMovementComponent.h"

AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	Fists = CreateDefaultSubobject<UFistsComponent>(TEXT("Fists"));
	WeaponController = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon Controller"));
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
	if (!OverlappedItem && EquippedWeapon)
	{
		Unequip();
	}
	else if (EquippedWeapon && OverlappedItem)
	{
		SwapItem(OverlappedItem);
	}
	else if (!EquippedWeapon && OverlappedItem)
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
	// Invoked from Binding
	if (AnimationState != EAnimationState::EAS_NoAnimation)
		return;

	if (CharacterState == ECharacterState::ECS_WithMeeleWeapon)
	{
		InitiateAttackWithWeapon();
	}
	if (CharacterState == ECharacterState::ECS_WithFireWeapon)
	{
		InitiateAttackWithFireWeapon();
	}
	if (CharacterState == ECharacterState::ECS_WithItem)
	{
		InitiateAttackWithItem();
	}
	if (CharacterState == ECharacterState::ECS_WithoutWeapon)
	{
		InitiateAttackWithoutWeapon();
	}
}

void AHeroCharacter::InitiateAttackWithItem()
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] InitiateAttackWithItem"));
	// Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(MontageAttack);
		FName SequenceName = "Throw1";
		AnimInstance->Montage_JumpToSection(SequenceName, MontageAttack);
		AnimationState = EAnimationState::EAS_AnimationInProgress;
	}
}
void AHeroCharacter::InitiateAttackWithWeapon()
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] InitiateAttackWithWeapon"));
	// Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(MontageAttack);
		FName SequenceName;
		switch (RandomSequence)
		{
			case 1:
				SequenceName = "Attack1";
				break;
			case 2:
				SequenceName = "Attack2";
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Unidentified animation montage number"));
				break;
		};
		AnimInstance->Montage_JumpToSection(SequenceName, MontageAttack);
		AnimationState = EAnimationState::EAS_AnimationInProgress;
	}
}

void AHeroCharacter::InitiateAttackWithoutWeapon()
{
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] InitiateAttackWithoutWeapon"));

	// Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(MontageAttack);
		FName SequenceName;
		switch (RandomSequence)
		{
			case 1:
				SequenceName = "Punch1";
				break;
			case 2:
				SequenceName = "Punch2";
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Unidentified animation montage number"));
				break;
		};
		AnimInstance->Montage_JumpToSection(SequenceName, MontageAttack);
	}

}

void AHeroCharacter::AttackEnd()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackEnd"));
	if (AWeapon* Weapon = Cast<AWeapon>(EquippedWeapon))
	{
		// TODO: why this does not throw err? 
		Weapon->EnableOverlappingEvents(false);
	}
	else if (IThrowableInterface* Throwable = Cast<IThrowableInterface>(EquippedItem))
	{
		FVector Direction = GetActorForwardVector();
		Throwable->Throw(Direction);
		EquippedItem = nullptr;
	}
	else if (AFireWeapon *FireWeapon = Cast<AFireWeapon>(EquippedWeapon))
	{

	}
	else if (!EquippedItem || !EquippedWeapon)
	{
		Fists->EnableOverlappingEvents(false);
	}	

	AnimationState = EAnimationState::EAS_NoAnimation;
}

void AHeroCharacter::AttackStart()
{
	// Called from ABP
	UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] AttackStart"));
	if (AFireWeapon *FireWeapon = Cast<AFireWeapon>(EquippedWeapon))
	{
		FireWeapon->FireFromGun();
	}
	else if (IWeaponInterface* Weapon = Cast<IWeaponInterface>(EquippedWeapon))
	{
		Weapon->EnableOverlappingEvents(true);
	}
	else if (!EquippedItem || !EquippedWeapon)
	{
		Fists->EnableOverlappingEvents(true);
	}	
	AnimationState = EAnimationState::EAS_AnimationInProgress;
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
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Unequip: %s"), *EquippedWeapon->GetName());
		CharacterState = ECharacterState::ECS_WithoutWeapon;
		EquippedWeapon->Unequip();
		EquippedWeapon = nullptr;
}

void AHeroCharacter::Equip(AItem* Item)
{
	AttachItemToSocket(Item, "RightHandSocket");
	OverlappedItem = nullptr;
	// Change to MeeleWeaapon
	if (AWeapon* Weapon = Cast<AWeapon>(Item))
	{
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Equip(): Equipping a weapon: %s"), *Weapon->GetName());
		EquippedWeapon = Weapon;
		Weapon->Equip();
		CharacterState = ECharacterState::ECS_WithMeeleWeapon;
	}
	else if (AFireWeapon* FireWeapon = Cast<AFireWeapon>(Item))
	{
		UE_LOG(LogClass, Log, TEXT("[HeroCharacter] Equip(): Equipping a fire weapon: %s"), *FireWeapon->GetName());
		EquippedWeapon = FireWeapon;
		FireWeapon->Equip();
		CharacterState = ECharacterState::ECS_WithFireWeapon;
	}
	else if (IPickupInterface* PickableItem = Cast<IPickupInterface>(Item))
	{
		UE_LOG(LogTemp, Display, TEXT("[HeroCharacter] Equip(): Equipping an item"));
		PickableItem->OnItemEquipped(*this);
		EquippedItem = Item;
		CharacterState = ECharacterState::ECS_WithItem;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] Equip(): Picking unidentified object"));
	}	
}

void AHeroCharacter::InitiateAttackWithFireWeapon()
{
	AFireWeapon *FireWeapon = Cast<AFireWeapon>(EquippedWeapon);
	if (FireWeapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[HeroCharacter] Trying to initiate attack with fire weapon, but FireWeapon is not set"));
		return;
	}
	if (CharacterIsMoving())
	{
		FireWeapon->FireFromGun();
	}
	else if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(MontageAttack);
		FName SequenceName = "FireWeapon";
		AnimInstance->Montage_JumpToSection(SequenceName, MontageAttack);
		AnimationState = EAnimationState::EAS_AnimationInProgress;
	}
}

bool AHeroCharacter::CharacterIsMoving()
{
	if (GetMovementComponent()->Velocity == FVector::ZeroVector)
		return false;
	return true;
}



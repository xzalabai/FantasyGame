#include "InputControlComponent.h"
#include "EnhancedInputComponent.h"
#include "HeroCharacter.h"
#include "EnhancedInputSubsystems.h"

UInputControlComponent::UInputControlComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInputControlComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInputControlComponent::TestM()
{

}

void UInputControlComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	TObjectPtr<AHeroCharacter> OwnerCharacter = Cast<AHeroCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("[UInputControlComponent] SetupPlayerInputComponent FAILED"));
		return;
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::Insert);
		/*EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::Jump);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::Reload);
		EnhancedInputComponent->BindAction(ReleaseAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::MouseRelease);
		EnhancedInputComponent->BindAction(InsertAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::Insert);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::InitiateAttack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::ToggleEquip);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::InitiateBlock);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, OwnerCharacter, &AHeroCharacter::InitiateDodge);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, OwnerCharacter, &AHeroCharacter::BlockEnd);*/
	}
}


#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "PublicEnums.h"
#include "Projectile.h"

AFireWeapon::AFireWeapon()
{	
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
}
 
void AFireWeapon::BeginPlay()
{
	Super::BeginPlay();
	Muzzle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void AFireWeapon::EnableOverlappingEvents(bool bEnable)
{
    return;    
}

void AFireWeapon::FireFromGun()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), Muzzle->GetComponentRotation(), SpawnParams);
		if (Projectile)
		{
            UE_LOG(LogTemp, Warning, TEXT("Fire shot"));
			Projectile->FireInDirection(Muzzle->GetForwardVector());
		}
	}
}

void AFireWeapon::PerformMontage(AHeroCharacter &Character, UAnimInstance &AnimInstance)
{
	if (Character.CharacterIsMoving())
	{
		FireFromGun();
		Character.AttackEnd();
	}
	else
	{
		FireFromGun();
		Character.AttackEnd();
		//AWeapon::PerformMontage(Character, AnimInstance);
	}
	
}

void AFireWeapon::AttackMontageStarted()
{
	AHeroCharacter* Character = GetOwnerCharacter();
	if (!Character)
		return;

	if (Character->CharacterIsMoving())
	{
		FireFromGun();
	}
}

void AFireWeapon::AttackMontageEnded()
{
	AHeroCharacter* Character = GetOwnerCharacter();
	if (!Character)
		return;
	if (!(Character->CharacterIsMoving()))
	{
		//FireFromGun();
	}
}

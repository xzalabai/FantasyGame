#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "PublicEnums.h"
#include "DrawDebugHelpers.h" // Include the debug drawing helpe

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

void AFireWeapon::FireFromWeapon()
{
	UCameraComponent* PlayerCamera = GetOwnerCharacter()->GetCharacterCamera();
	FVector WorldLocation = PlayerCamera->GetComponentLocation();
	FVector ForwardVector = PlayerCamera->GetForwardVector();

	FVector Start = WorldLocation;
    FVector End = Start + (ForwardVector * 2000); // Calculate the end point of the line trace
    FHitResult HitResult; // Store the result of the line trace
    // Define the collision parameters
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this); // Ignore the actor performing the trace
    // Define the trace channel you want to check against
    ECollisionChannel TraceChannel = ECC_WorldDynamic; // Replace with your desired channel
    // Perform the line trace
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        TraceChannel,
        CollisionParams
    );

	FVector SphereLocation = HitResult.ImpactPoint;
	float SphereRadius = 20.0f; 
	FColor SphereColor = FColor::Red; 
	DrawDebugSphere(
		GetWorld(),
		SphereLocation,
		SphereRadius,
		32,
		SphereColor,
		true,
		-1.0f 
	);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
	if (Projectile)
	{
		UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] FireFromGun"));
		Projectile->FireInDirection(ForwardVector);
	}
	--AmmoInMagazine;
}

void AFireWeapon::ReloadWeapon()
{
	if (AmmoCapacity <= 0)
	{
		return;
	}
	AmmoCapacity += AmmoInMagazine;
	AmmoInMagazine = FMath::Max(0, AmmoCapacity - MaxAmmoInMagazine);
	AmmoCapacity = FMath::Max(0, AmmoCapacity - MaxAmmoInMagazine);

	AHeroCharacter* Character = GetOwnerCharacter();
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	int8 RandomIndex = FMath::RandRange(0, ReloadAnimationSequenceName.Num() - 1);

	AItem::PerformMontage(Character, AnimInstance, ReloadAnimationSequenceName[RandomIndex], ReloadMontage);
}

void AFireWeapon::PerformMontage(AHeroCharacter *Character, UAnimInstance *AnimInstance)
{
	if (AmmoInMagazine <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] No ammo in magazine"));
		return;
	}

	if (Character->CharacterIsMoving())
	{
		FireFromWeapon();
		Character->AttackEnd();
	}
	else
	{
		FireFromWeapon();
		Character->AttackEnd();
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
		FireFromWeapon();
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

void AFireWeapon::PerformActionOnNotify()
{
	PerformBoxTrace();
}
#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "PublicEnums.h"
#include "TimerManager.h"
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

	FVector Start = WorldLocation;
    FVector End = WorldLocation + (PlayerCamera->GetForwardVector() * 2000); // Calculate the end point of the line trace
    FHitResult HitResult; // Store the result of the line trace
    FCollisionQueryParams CollisionParams;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

    CollisionParams.AddIgnoredActor(this);
    ECollisionChannel TraceChannel = ECC_WorldDynamic;
    // Perform the line trace
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        TraceChannel,
        CollisionParams
    );

	// DEBUG sphere at impact point
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

	// TODO: verify if End is good point if we didn't find target
	FRotator RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), bHit ? HitResult.ImpactPoint : End);
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), RotationTowardsTarget, SpawnParams);
	if (Projectile)
	{
		UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] Fire from weapon."));
		Projectile->FireInDirection(Projectile->GetActorForwardVector());
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
		UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] No ammo in magazine."));
		return;
	}
	if (GetWorldTimerManager().IsTimerActive(ClearTimerHandle))
	{
		UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] Timer is active."));
		return;
	}
	
	GetWorldTimerManager().SetTimer(WeaponTimerHandle, this, &AFireWeapon::FireFromWeapon, FireRate, true, 0);
}


void AFireWeapon::OnMouseRelease()
{
	if (GetWorldTimerManager().IsTimerActive(ClearTimerHandle))
	{
		return;
	}
	const float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(WeaponTimerHandle);
	if (TimeRemaining < FireRate)
	{
		GetWorldTimerManager().SetTimer(ClearTimerHandle, TimeRemaining, false, TimeRemaining);	
	}
	ClearWeaponTimer();
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

void AFireWeapon::ClearWeaponTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear Timer"));
	GetWorldTimerManager().ClearTimer(WeaponTimerHandle);
}

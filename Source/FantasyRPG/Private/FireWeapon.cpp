#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
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
	AHeroCharacter* Character = GetOwnerCharacter();
	bool bIsAiming = Character->IsAiming();

	UCameraComponent* PlayerCamera = Character->GetCharacterCamera();

	const FVector& Start = PlayerCamera->GetComponentLocation();
    const FVector& End = PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * 2000); // Calculate the end point of the line trace

    FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	ECollisionChannel TraceChannel = ECC_WorldDynamic;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Perform the line trace
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel, CollisionParams);

	// Debug sphere at impact point
	FVector& SphereLocation = HitResult.ImpactPoint;
	float SphereRadius = 20.0f; 
	DrawDebugSphere(GetWorld(), SphereLocation, SphereRadius, 32, FColor::Red, true, -1.0f);

	FVector FinalHitPoint = CreateShotDispersion(bHit ? HitResult.ImpactPoint : End, bIsAiming);
	FRotator RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), FinalHitPoint);
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), RotationTowardsTarget, SpawnParams);
	Projectile->FireInDirection(Projectile->GetActorForwardVector());
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

	AItem::PerformMontage(AnimInstance, ReloadAnimationSequenceName[RandomIndex], ReloadMontage);
}

void AFireWeapon::PerformMontage(UAnimInstance *AnimInstance)
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

FVector AFireWeapon::CreateShotDispersion(const FVector OriginalTarget, const bool bIsAiming)
{
	int16 dispersion = bIsAiming ? 20 : 60;
	float dispersionX = FMath::RandRange(-dispersion, dispersion);
	float dispersionY = FMath::RandRange(-dispersion, dispersion);
	float dispersionZ = FMath::RandRange(-dispersion, dispersion);

	return FVector(OriginalTarget.X + dispersionX, OriginalTarget.Y + dispersionY, OriginalTarget.Z + dispersionZ);
}

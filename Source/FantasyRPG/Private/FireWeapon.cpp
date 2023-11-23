#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
#include "ProjectilePoolComponent.h"
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
	ProjectilePool = CreateDefaultSubobject<UProjectilePoolComponent>(TEXT("ProjectilePool"));
}
 
void AFireWeapon::BeginPlay()
{
	Super::BeginPlay();
	Muzzle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ProjectilePool->CreatePool(ProjectileClass, MaxAmmoInMagazine);
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
}
void AFireWeapon::EnableOverlappingEvents(bool bEnable)
{
    return;    
}

void AFireWeapon::FireFromWeapon()
{
	const AHeroCharacter* Character = GetOwnerCharacter();
	const UCameraComponent* PlayerCamera = Character->GetCharacterCamera();
	FVector End;
	float AimDispersion = UKismetMathLibrary::NormalizeToRange(Character->GetAimSpread(), 10, 40);

	if (!Character->IsAiming() && !Character->CharacterIsMoving())
	{
		End = Muzzle->GetComponentLocation() + (Muzzle->GetForwardVector() * 2000);
	}
	else
	{
		End = PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * 2000);
	}
	const FVector Start = PlayerCamera->GetComponentLocation();
	FHitResult HitResult;

	bool bHitActor = CalculateShotEndPosition(Start, End, HitResult);
	
	FVector FinalHitPoint = CreateShotDispersion(bHitActor ? HitResult.ImpactPoint : End, AimDispersion);
	FRotator RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), FinalHitPoint);
	
	TObjectPtr<AProjectile> Projectile = ProjectilePool->GetActorFromPool();
	Projectile->OwnerWeapon = this;
	Projectile->SetActorLocation(Muzzle->GetComponentLocation());
	Projectile->SetActorRotation(RotationTowardsTarget);
	Projectile->FireInDirection(Projectile->GetActorForwardVector());
	Character->WeaponFired(AssetName);
	
	--AmmoInMagazine;
}

bool AFireWeapon::CalculateShotEndPosition(const FVector& Start, const FVector& End, FHitResult& OutHitResult)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_WorldDynamic, CollisionParams);

	// Debug sphere at impact point
	float SphereRadius = 20.0f;
	DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, SphereRadius, 32, FColor::Red, true, -1.0f);
	
	return bHit;
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

	const AHeroCharacter* Character = GetOwnerCharacter();
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
	const AHeroCharacter* Character = GetOwnerCharacter();
	if (!Character)
		return;

	if (Character->CharacterIsMoving())
	{
		FireFromWeapon();
	}
}

void AFireWeapon::AttackMontageEnded()
{
	const AHeroCharacter* Character = GetOwnerCharacter();
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
	GetWorldTimerManager().ClearTimer(WeaponTimerHandle);
}

FVector AFireWeapon::CreateShotDispersion(const FVector OriginalTarget, int16 Dispersion)
{
	float dispersionX = FMath::RandRange(-Dispersion, Dispersion);
	float dispersionY = FMath::RandRange(-Dispersion, Dispersion);
	float dispersionZ = FMath::RandRange(-Dispersion, Dispersion);

	return FVector(OriginalTarget.X + dispersionX, OriginalTarget.Y + dispersionY, OriginalTarget.Z + dispersionZ);
}

void AFireWeapon::ReturnToPool(TObjectPtr<AProjectile> Projectile)
{
	// TODO Change to callback
	ProjectilePool->InsertToPool(Projectile);
}

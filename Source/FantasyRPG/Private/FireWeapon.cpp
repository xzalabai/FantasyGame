#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
#include "AttributesComponent.h"
#include "ProjectilePoolComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "PublicEnums.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h" 
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

void AFireWeapon::OnItemEquipped(AHeroCharacter* MainCharacter)
{
	int AmmoFromInventory = MainCharacter->GetAttributes()->GetAmmoFromInventory(ItemType);
	UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] OnItemEquipped. %d"), AmmoFromInventory);
	AmmoCapacity = AmmoFromInventory;
	Super::OnItemEquipped(MainCharacter);
}

void AFireWeapon::FireFromWeapon()
{
	if (AmmoInMagazine <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("[AFireWeapon] No ammo in magazine."));
		return;
	}

	const AHeroCharacter* Character = GetOwnerCharacter();
	const UCameraComponent* PlayerCamera = Character->GetCharacterCamera();
	float AimDispersion = UKismetMathLibrary::NormalizeToRange(Character->GetAimSpread(), 10, 40);

	// Calculate end destination and shot dispersion
	FVector End = (!Character->IsAiming() && !Character->CharacterIsMoving())
		? Muzzle->GetComponentLocation() + (Muzzle->GetForwardVector() * 2000)
		: PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * 2000);

	FHitResult HitResult;
	bool bHitActor = CalculateShotEndPosition(PlayerCamera->GetComponentLocation(), End, HitResult);
	
	FVector FinalHitPoint = CreateShotDispersion(bHitActor ? HitResult.ImpactPoint : End, AimDispersion);
	FRotator RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), FinalHitPoint);
	
	// Set new projectile
	TObjectPtr<AProjectile> Projectile = ProjectilePool->GetActorFromPool();
	Projectile->OwnerWeapon = this;
	Projectile->SetActorLocation(Muzzle->GetComponentLocation());
	Projectile->SetActorRotation(RotationTowardsTarget);
	Projectile->FireInDirection(Projectile->GetActorForwardVector());
	
	// Get CB to Character
	Character->WeaponFired(AssetName);
	Character->GetAttributes()->DecreaseAmmo(ItemType, 1);
	
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

	// Ammo calculations
	AmmoCapacity += AmmoInMagazine;
	AmmoInMagazine = FMath::Max(0, AmmoCapacity - MaxAmmoInMagazine > 0 ? AmmoCapacity - MaxAmmoInMagazine : AmmoCapacity);
	AmmoCapacity = FMath::Max(0, AmmoCapacity - MaxAmmoInMagazine);

	// Perform Montage
	UAnimInstance* AnimInstance = GetOwnerCharacter()->GetMesh()->GetAnimInstance();
	int8 RandomIndex = FMath::RandRange(0, ReloadAnimationSequenceName.Num() - 1);
	AItem::PerformMontage(AnimInstance, ReloadAnimationSequenceName[RandomIndex], ReloadMontage);
}

void AFireWeapon::PerformMontage(UAnimInstance *AnimInstance)
{
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

#include "FireWeapon.h"
#include "Components/SceneComponent.h"
#include "HeroCharacter.h"
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

void AFireWeapon::FireFromGun()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UCameraComponent* PlayerCamera = GetOwnerCharacter()->GetCharacterCamera();
	FVector WorldLocation = PlayerCamera->GetComponentLocation();
	FVector ForwardVector = PlayerCamera->GetForwardVector();
	FVector ForwardVector2 = ForwardVector * 2000;
	FVector EndTracing = WorldLocation + ForwardVector2;


	// ------------------------------------------------------------------
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

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get hit ++++"));
		FVector SphereLocation = HitResult.ImpactPoint; // Change this to the desired location
		float SphereRadius = 20.0f; // Change this to the desired radius
		FColor SphereColor = FColor::Red; // Change this to the desired color
		DrawDebugSphere(
			GetWorld(),
			SphereLocation,
			SphereRadius,
			32,
			SphereColor,
			true, // Persistent (if true, the sphere will remain visible until explicitly cleared)
			-1.0f // Lifetime (-1.0f means the sphere will stay visible until explicitly cleared)
		);


		FVector A = Muzzle->GetComponentLocation()/* Your initial FVector */;
		FVector B = HitResult.ImpactPoint/* The target FVector whose yaw you want to match */;
		float YawDifference = (B - A).Rotation().Yaw;

		// Apply the yaw rotation to FVector A
		A = FRotator(0.0f, YawDifference, 0.0f).RotateVector(A);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), FRotator(0,0,0), SpawnParams);
		if (Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire shot"));
			Projectile->FireInDirection(ForwardVector);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing was hit ----"));
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

void AFireWeapon::PerformActionOnNotify()
{
	PerformBoxTrace();
}
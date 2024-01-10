#include "PrimitiveEnemy.h"
#include "AttributesComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

void APrimitiveEnemy::OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage)
{
	AEnemy::OnReceivedHit(HitImpactPoint, HitLocation, Attacker, Damage);
	
	UE_LOG(LogTemp, Display, TEXT("Primitive Enemy received a hit with %d damage. He is left with %d HP"), Damage, Attributes->GetHealth());

	bool bForwardHit = IsHitFromFront(HitImpactPoint);
	Attributes->IsAlive() ? ProcessHit(bForwardHit, HitImpactPoint, HitLocation) : ProcessDeath(bForwardHit, HitImpactPoint, HitLocation);

}

void APrimitiveEnemy::LayingDead()
{   
	UE_LOG(LogTemp, Display, TEXT("[APrimitiveEnemy] Laying Dead"));
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(false);
	}
	TurnOff();
}

void APrimitiveEnemy::ProcessHit(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation)
{   
	UE_LOG(LogTemp, Display, TEXT("[APrimitiveEnemy] ProcessHit"));
    //Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(AnimationMontage);
		FName SequenceName = "HitForward";
		AnimInstance->Montage_JumpToSection(SequenceName, AnimationMontage);
	}
}

void APrimitiveEnemy::ProcessDeath(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation)
{   
	UE_LOG(LogTemp, Display, TEXT("[APrimitiveEnemy] ProcessDeath"));
    //Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(DeadAnimationMontage);
		FName SequenceName = bForwardHit ? "DeathBackwards" : "DeathForward";
		AnimInstance->Montage_JumpToSection(SequenceName, AnimationMontage);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AEnemy::ProcessDeath(bForwardHit, HitImpactPoint, HitLocation);
}
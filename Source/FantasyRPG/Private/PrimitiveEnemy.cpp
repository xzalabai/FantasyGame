#include "PrimitiveEnemy.h"
#include "AttributesComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

void APrimitiveEnemy::OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage)
{
	AEnemy::OnReceivedHit(HitImpactPoint, HitLocation, Attacker, Damage);
	
	UE_LOG(LogTemp, Display, TEXT("Primitive Enemy received a hit with %d damage. He is left with %d HP"), Damage, Attributes->GetHealth());

	bool bForwardHit = IsHitFromFront(HitImpactPoint);
	if (Attributes->IsAlive())
	{
		ProcessHit(bForwardHit, HitImpactPoint, HitLocation);
	}
	else
	{
		ProcessDeath(bForwardHit, HitImpactPoint, HitLocation);
	}	
}

void APrimitiveEnemy::LayingDead()
{   
	UE_LOG(LogTemp, Display, TEXT("[APrimitiveEnemy] Laying Dead"));
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(false);
	}
	//GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
}

void APrimitiveEnemy::ProcessHit(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation)
{   
	UE_LOG(LogTemp, Display, TEXT("[APrimitiveEnemy] ProcessHit"));
	USkeletalMeshComponent *EnemyMesh = GetMesh();
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
		AnimInstance->Montage_Play(AnimationMontage);
		FName SequenceName = bForwardHit ? "DeathBackwards" : "DeathForward";
		AnimInstance->Montage_JumpToSection(SequenceName, AnimationMontage);
		SetDeathAnimationPose(SequenceName);
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AEnemy::ProcessDeath(bForwardHit, HitImpactPoint, HitLocation);
}

FName APrimitiveEnemy::GetDeathAnimationName()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		FName MontageName = AnimInstance->Montage_GetCurrentSection(AnimationMontage);
		return MontageName;
	}
	return "";
}

void APrimitiveEnemy::SetDeathAnimationPose(const FName AnimMontageName)
{
	if (AnimMontageName == "DeathBackwards")
	{
		DeathAnimationPose = EPrimitiveEnemyDeathAnimation::EPEDA_DeathBack;
	}
	else if (AnimMontageName == "DeathForward")
	{
		DeathAnimationPose = EPrimitiveEnemyDeathAnimation::EPEDA_DeathForward;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[APrimitiveEnemy]Unknown Death animation pose "));
	}
}

const EPrimitiveEnemyDeathAnimation APrimitiveEnemy::GetDeathAnimationPose()
{
	return DeathAnimationPose;
}

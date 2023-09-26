#include "PrimitiveEnemy.h"
#include "AttributesComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

void APrimitiveEnemy::OnReceivedHit(const FVector& ImpactPoint, int Damage)
{
	AEnemy::OnReceivedHit(ImpactPoint, Damage);
	
	UE_LOG(LogTemp, Display, TEXT("Primitive Enemy received a hit with %d damage. He is left with %d HP"), Damage, Attributes->GetHealth());

	bool bForwardHit = IsHitFromFront(ImpactPoint);
	if (Attributes->IsAlive())
	{
		ProcessHit(bForwardHit);
	}
	else
	{
		ProcessDeath(bForwardHit);
	}	
}

void APrimitiveEnemy::LayingDead()
{   
	UE_LOG(LogTemp, Warning, TEXT("Laying Dead"));
	//GetMesh()->SetCollisionProfileName("Ragdoll");
}

void APrimitiveEnemy::ProcessHit(bool bForwardHit)
{   
	UE_LOG(LogTemp, Warning, TEXT("[APrimitiveEnemy] ProcessHit"));
	USkeletalMeshComponent *EnemyMesh = GetMesh();
    //Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(AnimMontage);
		FName SequenceName = "HitForward";
		AnimInstance->Montage_JumpToSection(SequenceName, AnimMontage);
	}
}

void APrimitiveEnemy::ProcessDeath(bool bForwardHit)
{   
	UE_LOG(LogTemp, Warning, TEXT("[APrimitiveEnemy] ProcessDeath"));
    //Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(AnimMontage);
		FName SequenceName = bForwardHit ? "DeathBack" : "DeathForward";
		AnimInstance->Montage_JumpToSection(SequenceName, AnimMontage);
		SetDeathAnimationPose(SequenceName);
	}
	UE_LOG(LogTemp, Warning, TEXT("Death"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AEnemy::ProcessDeath();
}

FName APrimitiveEnemy::GetDeathAnimationName()
{
	return "DeathBack";
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		FName MontageName = AnimInstance->Montage_GetCurrentSection(AnimMontage);
		return MontageName;
	}
	return "";
}

void APrimitiveEnemy::SetDeathAnimationPose(const FName AnimMontageName)
{
	if (AnimMontageName == "DeathBack")
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

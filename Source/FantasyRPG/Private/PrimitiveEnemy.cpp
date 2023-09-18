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
	GetMesh()->SetCollisionProfileName("Ragdoll");
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
	UE_LOG(LogTemp, Warning, TEXT("Processss d"));
    USkeletalMeshComponent *EnemyMesh = GetMesh();
    //Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(AnimMontage);
		FName SequenceName = bForwardHit ? "DeathBack" : "DeathForward";
		AnimInstance->Montage_JumpToSection("DeathBack", AnimMontage);
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AEnemy::ProcessDeath();
}

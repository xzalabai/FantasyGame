#include "RagdollEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributesComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void ARagdollEnemy::OnReceivedHit(const FVector& ImpactPoint, int Damage)
{
	AEnemy::OnReceivedHit(ImpactPoint, Damage);
	
	UE_LOG(LogTemp, Display, TEXT("[ARagdollEnemy] Received a hit with %d damage. He is left with %d HP"), Damage, Attributes->GetHealth());

	bool bForwardHit = IsHitFromFront(ImpactPoint);
    //ProcessHit(bForwardHit);
    //return;
	if (Attributes->IsAlive())
	{
		ProcessHit(bForwardHit);
	}
	else
	{
		ProcessDeath(bForwardHit);
	}	
}

void ARagdollEnemy::ProcessHit(bool bForwardHit)
{   
	UE_LOG(LogTemp, Warning, TEXT("[ARagdollEnemy] ProcessHit ---"));
	USkeletalMeshComponent *EnemyMesh = GetMesh();
    
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
        UE_LOG(LogTemp, Display, TEXT("Play "));
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(AnimMontage);
		FName SequenceName = "HitForwardSmall";
		AnimInstance->Montage_JumpToSection(SequenceName, AnimMontage);
	}
}

void ARagdollEnemy::ProcessDeath(bool bForwardHit)
{   
	UE_LOG(LogTemp, Warning, TEXT("[ARagdollEnemy] ProcessDeath"));

    USkeletalMeshComponent *EnemyMesh = GetMesh();
    EnemyMesh->SetCollisionProfileName("Ragdoll");
    EnemyMesh->SetSimulatePhysics(true);

    //Play animation
	// if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	// {
	// 	int8 RandomSequence = FMath::RandRange(1, 2);
	// 	AnimInstance->Montage_Play(AnimMontage);
	// 	FName SequenceName = bForwardHit ? "DeathBack" : "DeathForward";
	// 	AnimInstance->Montage_JumpToSection(SequenceName, AnimMontage);
	// 	SetDeathAnimationPose(SequenceName);
	// }
	AEnemy::ProcessDeath();
}



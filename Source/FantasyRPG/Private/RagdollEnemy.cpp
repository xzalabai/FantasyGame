#include "RagdollEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributesComponent.h"
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EquipableInterface.h"


void ARagdollEnemy::PerformActionOnNotify()
{
	// Called from ABP
	IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem());
	if (Item)
	{
		Item->PerformActionOnNotify();
	}
}


void ARagdollEnemy::OnReceivedHit(const FVector& ImpactPoint, AActor* Attacker, int Damage)
{
	AEnemy::OnReceivedHit(ImpactPoint, Attacker, Damage);
	
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
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(AnimMontage);
		FName SequenceName = "HitForwardSmall1";
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
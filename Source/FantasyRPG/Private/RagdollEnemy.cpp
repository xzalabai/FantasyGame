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
#include <Kismet\KismetMathLibrary.h>


void ARagdollEnemy::PerformActionOnNotify()
{
	// Called from ABP
	IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem());
	if (Item)
	{
		Item->PerformActionOnNotify();
	}
}


void ARagdollEnemy::OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage)
{
	AEnemy::OnReceivedHit(HitImpactPoint, HitLocation, Attacker, Damage);
	
	UE_LOG(LogTemp, Display, TEXT("[ARagdollEnemy] Received a hit with %d damage. He is left with %d HP"), Damage, Attributes->GetHealth());

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

void ARagdollEnemy::ProcessHit(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation)
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

void ARagdollEnemy::ProcessDeath(bool bForwardHit, const FVector& ImpactPoint, const FVector& HitLocation)
{   
	UE_LOG(LogTemp, Warning, TEXT("[ARagdollEnemy] ProcessDeath"));

    USkeletalMeshComponent *EnemyMesh = GetMesh();
    EnemyMesh->SetCollisionProfileName("Ragdoll");
    EnemyMesh->SetSimulatePhysics(true);
	EnemyMesh->AddImpulseAtLocation(UKismetMathLibrary::TransformDirection(GetActorTransform(), FVector(-ImpactPoint.X * 10, -ImpactPoint.Y * 10, -ImpactPoint.Z * 10)), HitLocation);
	AEnemy::ProcessDeath(bForwardHit, ImpactPoint, HitLocation);
}
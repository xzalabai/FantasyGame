// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveEnemy.h"
#include "DrawDebugHelpers.h"

void APrimitiveEnemy::OnReceivedHit(const FVector& ImpactPoint)
{
    //FVector OutHitDirection = CalculateVectorDirection(ImpactPoint, GetActorLocation());
    USkeletalMeshComponent *EnemyMesh = GetMesh();
    EnemyMesh->SetGenerateOverlapEvents(false);
    UE_LOG(LogTemp, Warning, TEXT("Ragdoll enemy received a hit"));
    // Play animation
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		int8 RandomSequence = FMath::RandRange(1, 2);
		AnimInstance->Montage_Play(DeathMontage);
		FName SequenceName = "DeathBack";
		AnimInstance->Montage_JumpToSection(SequenceName, DeathMontage);
	}
    Death();

}

void APrimitiveEnemy::LayingDead()
{   

}

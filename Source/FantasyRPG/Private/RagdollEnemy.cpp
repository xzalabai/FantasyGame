// Fill out your copyright notice in the Description page of Project Settings.


#include "RagdollEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

void ARagdollEnemy::OnReceivedHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Log, TEXT("Impact Point: %s"), *ImpactPoint.ToString());
    FVector OutHitDirection = CalculateVectorDirection(ImpactPoint, GetActorLocation());
    DrawDebugSphere(GetWorld(), OutHitDirection, 10, 12,FColor::Green, true, -1.0f,0, 2.0f);

    USkeletalMeshComponent *EnemyMesh = GetMesh();
    EnemyMesh->SetCollisionProfileName("Ragdoll");
    
    EnemyMesh->SetSimulatePhysics(true);
    EnemyMesh->AddImpulse(OutHitDirection);
    EnemyMesh->SetGenerateOverlapEvents(false);
    Death();
}




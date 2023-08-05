// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
    
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnReceivedHit(const FVector& ImpactDirection)
{
	UE_LOG(LogTemp, Warning, TEXT("This method should be overriden"));
}

void AEnemy::Death()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector AEnemy::CalculateVectorDirection(FVector PointA, FVector PointB)
{
    // Direction from PointA to PointB
    FVector Direction = (PointB - PointA).GetSafeNormal();
    // Distance between PointA and PointB
    float DistanceFactor = 1000;
    float Distance = FVector::Distance(PointA, PointB) * DistanceFactor;
    // Position of the aligned point by adding the direction multiplied by the distance to PointB
    FVector Multiplier(Direction.X * Distance, Direction.Y * Distance, Direction.Z);
    return PointB + Multiplier;
}


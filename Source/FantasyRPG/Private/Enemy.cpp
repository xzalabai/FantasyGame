
#include "Enemy.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "AttributesComponent.h"
#include "Item.h"
#include "Components/CapsuleComponent.h"
#include "EquipableInterface.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
    Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
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

void AEnemy::OnReceivedHit(const FVector& ImpactDirection, AActor* Attacker, int Damage)
{
	Attributes->DecreaseHealth(Damage);
}

void AEnemy::BlockAttack(const FVector& ImpactDirection, int Damage)
{
    // TODO: Implement Enemy blocking attack
    UE_LOG(LogTemp, Display, TEXT("[AEnemy] Enemy blocking missing"));
}

void AEnemy::ProcessHit()
{
    
}

void AEnemy::ProcessDeath()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetGenerateOverlapEvents(false);
}

void AEnemy::PerformActionOnNotify()
{
    IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem());
    Item->PerformActionOnNotify();
}

void AEnemy::AttackStart()
{
    IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem());
    Item->AttackMontageStarted();
}

void AEnemy::AttackEnd()
{
    IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem());
    Item->AttackMontageEnded();
}

void AEnemy::ReloadEnd()
{
    return;
}

void AEnemy::OnPerfectBlockReceived()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    PlayAnimMontage(AnimMontage, 1.0f, FName(TEXT("HitForwardSmall")));
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

bool AEnemy::IsHitFromFront(const FVector &ImpactPoint)
{
	FVector OutHitDirection = CalculateVectorDirection(ImpactPoint, GetActorLocation());
	float DotProduct = FVector::DotProduct(OutHitDirection, GetActorLocation());
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(),OutHitDirection, GetActorLocation(), FLinearColor::Red, 40, 10);
	return (DotProduct >= 0) ?  true : false;
}


AItem* AEnemy::GetEquippedItem()
{
    return EquippedItem;
}
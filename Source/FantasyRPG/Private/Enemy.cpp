
#include "Enemy.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "AttributesComponent.h"
#include "Item.h"
#include "Components/CapsuleComponent.h"
#include "EquipableInterface.h"
#include "Kismet\GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
    Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* HeroCharacterActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemy::StaticClass()))
	{
		if (AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(HeroCharacterActor))
		{
			OnEnemyAttackStarted.AddUObject(HeroCharacter, &AHeroCharacter::EnemyAttackStarted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AEnemy] Unable to fetch Hero Character for %s."), *GetClass()->GetName());
	}
}

void AEnemy::OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage)
{
	Attributes->DecreaseHealth(Damage);
}

void AEnemy::BlockAttack(const FVector& ImpactDirection, int Damage)
{
    // TODO: Implement Enemy blocking attack
    UE_LOG(LogTemp, Warning, TEXT("[AEnemy] Enemy blocking missing"));
}

void AEnemy::ProcessHit(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation)
{
    
}

void AEnemy::InitiateAttack()
{
    UE_LOG(LogTemp, Display, TEXT("[AEnemy] InitiateAttack"));
    int8 RandomSequence = FMath::RandRange(0, AttackAnimationSequence.Num() - 1);
    FName SequenceName = AttackAnimationSequence[RandomSequence];
    InitiateAttack_BP(AnimationMontage, SequenceName);
}

void AEnemy::ProcessDeath(bool bForwardHit, const FVector& ImpactPoint, const FVector& HitLocation)
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetGenerateOverlapEvents(false);
    if (Controller)
    {
        Controller->UnPossess();
    }
}

void AEnemy::PerformActionOnNotify()
{
    UE_LOG(LogTemp, Display, TEXT("[AEnemy] Try PerformActionOnNotify"));
    if (IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem()))
    {
        UE_LOG(LogTemp, Display, TEXT("[AEnemy] PerformActionOnNotify"));
        Item->PerformActionOnNotify();
    }
    
}

void AEnemy::AttackStart()
{
    UE_LOG(LogTemp, Display, TEXT("[AEnemy] AttackStart"));
    if (IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem()))
    {
        Item->AttackMontageStarted();
    }
    OnEnemyAttackStarted.Broadcast();
}

void AEnemy::AttackEnd()
{
    UE_LOG(LogTemp, Display, TEXT("[AEnemy] AttackEnd"));
    if (IEquipableInterface* Item = Cast<IEquipableInterface>(GetEquippedItem()))
    {
        Item->AttackMontageEnded();
    }
    
}

void AEnemy::ReloadEnd()
{
    return;
}

void AEnemy::OnPerfectBlockReceived()
{
    int8 RandomIndex = FMath::RandRange(0, PerfectBlockReceivedAnimationSequence.Num() - 1);
    PlayAnimMontage(AnimationMontage, 1.0f, PerfectBlockReceivedAnimationSequence[RandomIndex]);
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
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "Enemy.generated.h"

class UAttributesComponent;
class AHeroCharacter;
class AItem;


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEnemyAttackStartedDelegate, bool bStart, int8 enemyID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class FANTASYRPG_API AEnemy : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void OnReceivedHit(const FVector& HitImpactPoint, const FVector& HitLocation, AActor* Attacker, int Damage) override;
	void BlockAttack(const FVector& ImpactDirection, int Damage) override;
	AItem* GetEquippedItem();

	FOnEnemyAttackStartedDelegate OnEnemyAttack;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttackEndDelegate OnAttackEnd;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category=Montages)
	TObjectPtr<UAnimMontage> AnimationMontage;
	UPROPERTY(EditAnywhere, Category = "Montages")
	TArray<FName> HitReactionAnimationSequence;
	UPROPERTY(EditAnywhere, Category = "Montages")
	TArray<FName> AttackAnimationSequence;
	UPROPERTY(EditAnywhere, Category = "Montages")
	TArray<FName> PerfectBlockReceivedAnimationSequence;
	UPROPERTY(EditAnywhere, Category = "Montages")
	TArray<FName> DeathAnimationSequence;
	UPROPERTY(BlueprintReadWrite)
	AItem* EquippedItem;
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;
	FTimerHandle TimerHandle;
	FVector CalculateVectorDirection(FVector PointA, FVector PointB);
	bool IsHitFromFront(const FVector &ImpactPoint);
	void DestroyEnemy();
	const TObjectPtr<AHeroCharacter> FetchHeroCharacter() const;
	virtual void ProcessHit(bool bForwardHit, const FVector& HitImpactPoint, const FVector& HitLocation);
	virtual void ProcessDeath(bool bForwardHit, const FVector& ImpactPoint, const FVector& HitLocation);
	virtual void PerformActionOnNotify() override;
	virtual void AttackStart() override;
	virtual void AttackEnd() override;
	virtual void ReloadEnd() override;
	virtual void OnPerfectBlockReceived() override;
	virtual void InitiateAttack() override;
	virtual void LayingDead() override {};
	UFUNCTION(BlueprintImplementableEvent) // TODO: temporary, change to C++ only
	void InitiateAttack_BP(UAnimMontage* MontageToPlay, FName SectionName);
};


	
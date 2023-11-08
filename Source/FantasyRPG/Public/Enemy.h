#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "Enemy.generated.h"

class UAttributesComponent;
class AItem;

DECLARE_MULTICAST_DELEGATE(FOnEnemyAttackStartedDelegate);

UCLASS()
class FANTASYRPG_API AEnemy : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void OnReceivedHit(const FVector& ImpactDirection, AActor* Attacker, int Damage) override;
	void BlockAttack(const FVector& ImpactDirection, int Damage) override;
	AItem* GetEquippedItem();

	FOnEnemyAttackStartedDelegate OnEnemyAttackStarted;

protected:
	UPROPERTY(EditAnywhere, Category=Montages)
	UAnimMontage* AnimMontage;
	UPROPERTY(BlueprintReadWrite)
	AItem* EquippedItem;

	FVector CalculateVectorDirection(FVector PointA, FVector PointB);
	bool IsHitFromFront(const FVector &ImpactPoint);
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;	
	void ProcessHit();
	void ProcessDeath();
	virtual void PerformActionOnNotify() override;
	virtual void AttackStart() override;
	virtual void AttackEnd() override;
	virtual void ReloadEnd() override;
	virtual void OnPerfectBlockReceived() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<FName> PerfectBlockReceivedMontageName;
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "Enemy.generated.h"

class UAttributesComponent;
class AItem;

UCLASS()
class FANTASYRPG_API AEnemy : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnReceivedHit(const FVector& ImpactDirection, int Damage) override;
	AItem* GetEquippedItem();

protected:
	UPROPERTY(EditAnywhere, Category=Montages)
	UAnimMontage* AnimMontage;
	UPROPERTY(BlueprintReadWrite)
	AItem* EquippedItem;

	virtual void BeginPlay() override;
	FVector CalculateVectorDirection(FVector PointA, FVector PointB);
	bool IsHitFromFront(const FVector &ImpactPoint);
	UPROPERTY(EditAnywhere, Category = "Components")
	UAttributesComponent* Attributes;	
	void ProcessHit();
	void ProcessDeath();
};



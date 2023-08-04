#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "Enemy.generated.h"

UCLASS()
class FANTASYRPG_API AEnemy : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;
	FVector CalculateVectorDirection(FVector PointA, FVector PointB);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnReceivedHit(const FVector& ImpactDirection) override;
	virtual void Death();

};



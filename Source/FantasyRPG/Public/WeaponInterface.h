#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API IWeaponInterface
{
	GENERATED_BODY()
public:
	virtual void EnableOverlappingEvents(bool Enable) = 0;
	virtual void PerformMontage(class AHeroCharacter &Character, class UAnimInstance &AnimInstance) = 0;
	virtual void AttackMontageStarted() = 0;
	virtual void AttackMontageEnded() = 0;
	UFUNCTION(BlueprintCallable, Category = "Loot")
	virtual void PerformActionOnNotify() = 0;
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipableInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UEquipableInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API IEquipableInterface
{
	GENERATED_BODY()

	public:
	virtual void EnableOverlappingEvents(bool Enable) = 0;
	virtual void PerformMontage(class AHeroCharacter *Character, class UAnimInstance *AnimInstance) = 0;
	//virtual void PerformOnRelease() = 0;
	virtual void AttackMontageStarted() = 0;
	virtual void AttackMontageEnded() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void PerformActionOnNotify() = 0;
};

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
	virtual void PerformMontage(class UAnimInstance *AnimInstance, FName MontageName = "", class UAnimMontage* AnimMontage = nullptr) = 0;
	//virtual void PerformOnRelease() = 0;
	virtual void AttackMontageStarted() = 0;
	virtual void AttackMontageEnded() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void PerformActionOnNotify() = 0;
};

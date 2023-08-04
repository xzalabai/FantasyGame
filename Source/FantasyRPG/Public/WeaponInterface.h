#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API IWeaponInterface
{
	GENERATED_BODY()
public:
	virtual void EnableOverlappingEvents(bool Enable) = 0;
};

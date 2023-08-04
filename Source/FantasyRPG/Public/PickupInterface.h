#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeroCharacter.h"
#include "PickupInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API IPickupInterface
{
	GENERATED_BODY()
public:
	virtual void OnItemEquipped(AHeroCharacter &MainCharacter) = 0;
};

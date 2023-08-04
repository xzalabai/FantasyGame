#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponInterface.h"
#include "FireWeapon.generated.h"

UCLASS()
class FANTASYRPG_API AFireWeapon : public AItem, public IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void EnableOverlappingEvents(bool Enable) override;
};

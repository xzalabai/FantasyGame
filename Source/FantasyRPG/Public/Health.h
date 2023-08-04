// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickupInterface.h"
#include "Health.generated.h"

UCLASS()
class FANTASYRPG_API AHealth : public AItem, public IPickupInterface
{
private:
	AHealth();
	UPROPERTY(EditAnywhere)
	int Health;

	GENERATED_BODY()
	virtual void OnItemEquipped(AHeroCharacter &MainCharacter) override;
};



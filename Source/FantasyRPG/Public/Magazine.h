// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Magazine.generated.h"

UCLASS()
class FANTASYRPG_API AMagazine : public AItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int Size = 10;
	virtual void OnItemEquipped(AHeroCharacter* MainCharacter) override;
};

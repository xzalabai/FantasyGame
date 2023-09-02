// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API ICharacterInterface
{
	GENERATED_BODY()
public:
	virtual void OnReceivedHit(const FVector& ImpactDirection, int Damage) = 0;
};

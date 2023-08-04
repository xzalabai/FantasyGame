// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Fist.generated.h"

UCLASS()
class FANTASYRPG_API AFist : public AWeapon
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};


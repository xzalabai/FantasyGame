// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ThrowableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UThrowableInterface : public UInterface
{
	GENERATED_BODY()
};

class FANTASYRPG_API IThrowableInterface
{
	GENERATED_BODY()

public:
	virtual void Throw(FVector& Direction) = 0;
	virtual void OnThrowableOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) = 0;
};

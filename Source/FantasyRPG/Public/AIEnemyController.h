// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

UCLASS()
class FANTASYRPG_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
protected:
	void BeginPlay();
	void Tick(float Dt);
private:
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehavior;
};

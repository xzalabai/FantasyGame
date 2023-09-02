// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();
	UPROPERTY(EditAnywhere)
	int Health;
	UPROPERTY(EditAnywhere)
	int MaxHealth;

	FORCEINLINE int GetHealth() const { return Health; }
	FORCEINLINE bool IsAlive() const { return Health > 0; }
	FORCEINLINE void AddHealth(int NewHealth) { UE_LOG(LogTemp, Display, TEXT("HP added %d"), NewHealth); Health += NewHealth; }
	void DecreaseHealth(int DecreaseHealth);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PublicEnums.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributesComponent();
	UPROPERTY(EditAnywhere)
	int Health;
	UPROPERTY(EditAnywhere)
	int MaxHealth;
	UPROPERTY(VisibleAnywhere)
	TMap<EItemType, int> Ammo	;
	FORCEINLINE int GetHealth() const { return Health; }
	FORCEINLINE bool IsAlive() const { return Health > 0; }
	FORCEINLINE void AddHealth(int NewHealth) { UE_LOG(LogTemp, Display, TEXT("HP added %d"), NewHealth); Health += NewHealth; }
	void DecreaseHealth(int DecreaseHealth);
	void AddToAmmoInventory(const EItemType ItemType, const int Size);
	int GetAmmoFromInventory(const EItemType ItemType) const;
	void DecreaseAmmo(const EItemType ItemType, const int Size);
};

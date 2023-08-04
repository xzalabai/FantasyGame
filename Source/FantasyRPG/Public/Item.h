// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PublicEnums.h"
#include "Item.generated.h"


class UStaticMeshComponent;
class UCapsuleComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class FANTASYRPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere);
	UNiagaraComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere);
	USphereComponent* TriggerCollider;
	UPROPERTY(EditDefaultsOnly);
	EItemType ItemType;
	UPROPERTY(EditAnywhere);
	bool bAutoEquip = false;
protected:

	UPROPERTY(EditInstanceOnly)	
	EItemState ItemState = EItemState::EIS_LayingOnGround;
public:	
	virtual void Equip();
	virtual void Unequip();
	void AttachToSocket(USkeletalMeshComponent* MeshComp, FName SocketName);
	FORCEINLINE bool IsAutoEquip() const { return bAutoEquip;}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FistsComponent.generated.h"

class AHeroCharacter;
class AFist;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UFistsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFistsComponent();
	void RegisterHandColliders();
	void EnableOverlappingEvents(bool bEnable);
	UFUNCTION(BlueprintCallable)
	void PerformBoxTraceOnFists();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	AFist* LeftHand;
	UPROPERTY()
	AFist* RightHand;
	UPROPERTY()
	AFist* RightHandRef;
	UPROPERTY()
	AFist* LeftHandRef;
	AHeroCharacter* GetHero();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFist> FistBPClass;
};

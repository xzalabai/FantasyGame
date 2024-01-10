// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Engine/DataAsset.h"
#include "Weapon.h"
#include "DAItem.generated.h"

USTRUCT(BlueprintType)
struct FDAItemInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AssetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* AssetThumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAvailableToInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AItem> ItemType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsBuidFromItinerary;
};

UCLASS(BlueprintType)
class FANTASYRPG_API UDAItem : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDAItemInfo DAItemInfo;
};

USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDAItem* DAItemInfo;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	int Damage;

};

USTRUCT()
struct FFireWeaponData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = "Item Data")
    TSubclassOf<AProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    UAnimMontage* ReloadMontage;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    TArray<FName> ReloadAnimationSequenceName;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    int32 AmmoInMagazine;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    int32 MaxAmmoInMagazine;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    int32 AmmoCapacity;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    float FireRate;
};

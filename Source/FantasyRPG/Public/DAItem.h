// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Engine/DataAsset.h"
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

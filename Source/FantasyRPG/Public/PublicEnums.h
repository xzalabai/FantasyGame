// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_LayingOnGround UMETA(DisplayName = "Item laying on a ground"),
	EIS_Equipped UMETA(DisplayName = "Item is posessed/equiped")
};

UENUM(BlueprintType)
enum class EAnimationState : uint8
{
	EAS_AnimationInProgress UMETA(DisplayName = "Animation in progress"),
	EAS_NoAnimation UMETA(DisplayName = "No animation in progress")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_NoItem UMETA(DisplayName = "No Item"),
	EIT_Item UMETA(DisplayName = "Item"),
	EIT_Melee UMETA(DisplayName = "Melee"),
	EIT_Pistol UMETA(DisplayName = "Pistol"),
	EIT_Smg UMETA(DisplayName = "SMG"),
	EIT_Rifle UMETA(DisplayName = "Rifle"),
	EIT_NotSpecified UMETA(DisplayName = "Not Specified"),
};
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
enum class ECharacterState : uint8
{
	ECS_WithoutWeapon UMETA(DisplayName = "Without Weapon"),
	ECS_WithMeeleWeapon UMETA(DisplayName = "With Meele Weapon"),
	ECS_WithFireWeapon UMETA(DisplayName = "With Fire Weapon"),
	ECS_WithItem UMETA(DisplayName = "With Item")
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
	EIT_Health UMETA(DisplayName = "Health Item"),
	EIT_Weapon UMETA(DisplayName = "Weapon Item"),
};

UENUM(BlueprintType)
enum class EPrimitiveEnemyDeathAnimation : uint8
{
	EPEDA_DeathBack UMETA(DisplayName = "Death Back"),
	EPEDA_DeathForward UMETA(DisplayName = "Death Forward"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Pistol UMETA(DisplayName = "Pistol Magazine"),
	EWT_Smg UMETA(DisplayName = "SMG Magazine"),
	EWT_Rifle UMETA(DisplayName = "Rifle Magazine"),
};
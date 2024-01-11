// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FantasyRPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYRPG_API AFantasyRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void WarpTime(const float TimeDilation) const;
};

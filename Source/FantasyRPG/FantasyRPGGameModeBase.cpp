// Copyright Epic Games, Inc. All Rights Reserved.


#include "FantasyRPGGameModeBase.h"
#include "Kismet\GameplayStatics.h"


void AFantasyRPGGameModeBase::WarpTime(const float TimeDilation) const
{
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"
#include "AttributesComponent.h"

AHealth::AHealth()
{
    bAutoEquip = true;
}

void AHealth::OnItemEquipped(AHeroCharacter& MainCharacter)
{
    MainCharacter.GetAttributes()->AddHealth(Health);
    Destroy();
}


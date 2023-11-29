#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::DecreaseHealth(int Amount)
{
	Health = (Health - Amount) <= 0 ? 0 : (Health - Amount); 	
}

int UAttributesComponent::GetAmmoFromInventory(const EItemType ItemType) const
{
	return Ammo.Contains(ItemType) ? Ammo[ItemType] : 0;
}

void UAttributesComponent::AddToAmmoInventory(const EItemType ItemType, const int Size)
{
	Ammo.Contains(ItemType) ? Ammo.Add(ItemType, Size + Ammo[ItemType]) : Ammo.Add(ItemType, Size);
}

void UAttributesComponent::DecreaseAmmo(const EItemType ItemType, const int Size)
{
	Ammo.Contains(ItemType) ? Ammo.Add(ItemType, Ammo[ItemType] - Size) : Ammo.Add(ItemType, 0);
}


#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::DecreaseHealth(int Amount)
{
	Health = (Health - Amount) <= 0 ? 0 : (Health - Amount); 	
}

int UAttributesComponent::GetAmmoFromInventory(const EWeaponType WeaponType) const
{
	return Ammo.Contains(WeaponType) ? Ammo[WeaponType] : 0;
}

void UAttributesComponent::AddToAmmoInventory(const EWeaponType WeaponType, const int Size)
{
	Ammo.Contains(WeaponType) ? Ammo.Add(WeaponType, Size + Ammo[WeaponType]) : Ammo.Add(WeaponType, Size);
}

void UAttributesComponent::DecreaseAmmo(const EWeaponType WeaponType, const int Size)
{
	Ammo.Contains(WeaponType) ? Ammo.Add(WeaponType, Ammo[WeaponType] - Size) : Ammo.Add(WeaponType, 0);
}


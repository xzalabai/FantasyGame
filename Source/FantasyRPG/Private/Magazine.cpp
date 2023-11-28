#include "Magazine.h"
#include "Weapon.h"
#include "AttributesComponent.h"

void AMagazine::OnItemEquipped(AHeroCharacter* MainCharacter)
{
	MainCharacter->GetAttributes()->AddToAmmoInventory(WeaponTypeMagazine, Size);
	Destroy();
}

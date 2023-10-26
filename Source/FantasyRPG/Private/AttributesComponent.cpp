#include "AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributesComponent::DecreaseHealth(int Amount)
{
	X.Add(3);
	Health = (Health - Amount) <= 0 ? 0 : (Health - Amount); 	
}
#include "InventoryComponent.h"
#include "Item.h"
#include "DAItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


bool UInventoryComponent::InsertToInventory(AItem* Item)
{
	// if (MaxSize == InventoryItems.Num())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent] Unable to insert more items"));
	// 	return false;
	// }

	UDAItem* DAItem = Item->DAItem;
	UE_LOG(LogTemp, Display, TEXT("[UInventoryComponent] Adding to inventory %s"), *(DAItem->DAItemInfo.AssetName));
	InventoryItems.Add(DAItem);

	return true;
}

void UInventoryComponent::OnComponentDestroyed(bool B)
{
	InventoryItems.Empty();
	Super::OnComponentDestroyed(B);
}

void UInventoryComponent::ThrowFromItinerary()
{
	//NewObject<AItem>();
}
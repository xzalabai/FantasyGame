#include "InventoryComponent.h"
#include "Item.h"
#include "DAItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


bool UInventoryComponent::InsertToInventory(const AItem* Item)
{
	// if (MaxSize == InventoryItems.Num())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent] Unable to insert more items"));
	// 	return false;
	// }

	//UDAItem* DAItem = Item->DAItem;
	UDAItem* NewItem = DuplicateObject<UDAItem>(Item->DAItem, this);
	UE_LOG(LogTemp, Display, TEXT("[UInventoryComponent] Adding to inventory %s"), *(NewItem->DAItemInfo.AssetName));
	InventoryItems.Add(NewItem);
	return true;
}

void UInventoryComponent::OnComponentDestroyed(bool B)
{
	InventoryItems.Empty();
	Super::OnComponentDestroyed(B);
}

bool UInventoryComponent::RemoveFromInventory(const UDAItem* DAItem)
{
	UE_LOG(LogTemp, Display, TEXT("[UInventoryComponent] RemoveFromInventory %s"), *(DAItem->DAItemInfo.AssetName));
	
	if (InventoryItems.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("[UInventoryComponent] Unable to pop from inventory."));
		return false;
	}

	int32 Index;
	if (InventoryItems.Find(DAItem, Index))
	{
		UE_LOG(LogTemp, Display, TEXT("[UInventoryComponent] RemoveFromInventory Item was found on: %d"), Index);
		
		// Spawn item
		AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(AItem::StaticClass(), GetOwner()->GetTransform());
		Item->DAItem->DAItemInfo = DAItem->DAItemInfo;
		Item->FinishSpawning(GetOwner()->GetTransform());
		
		// Empty inventory slot
		InventoryItems.RemoveAt(Index);

		return true;
	}

	return false;
}
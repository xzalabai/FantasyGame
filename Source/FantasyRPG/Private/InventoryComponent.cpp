#include "InventoryComponent.h"
#include "Item.h"
#include "DAItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

UInventoryComponent::~UInventoryComponent()
{
	InventoryItems.Empty();
}


bool UInventoryComponent::InsertToInventory(const AItem* Item)
{
	// if (MaxSize == InventoryItems.Num())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("[UInventoryComponent] Unable to insert more items"));
	// 	return false;
	// }

	TObjectPtr<UDAItem> NewUDAItem = BuildUDAItemData(Item);
	UE_LOG(LogTemp, Display, TEXT("[UInventoryComponent] Adding to inventory %s"), *(NewUDAItem->DAItemInfo.AssetName));
	InventoryItems.Add(NewUDAItem);
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

		// Spawn item (either pick predefined subclass or default - AItem class)
		UClass* ClassToBeSpawned = DAItem->DAItemInfo.ItemType != nullptr ? DAItem->DAItemInfo.ItemType : AItem::StaticClass();
		AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(ClassToBeSpawned, GetOwner()->GetTransform());
		Item->DAItem = DuplicateObject<UDAItem>(DAItem, this);
		Item->FinishSpawning(GetOwner()->GetTransform());
		
		// Empty inventory slot
		InventoryItems.RemoveAt(Index);

		return true;
	}

	return false;
}

TObjectPtr<UDAItem> UInventoryComponent::BuildUDAItemData(const AItem* Item) const
{
	TObjectPtr<UDAItem> DAItem = NewObject<UDAItem>();
	DAItem->DAItemInfo.AssetName = Item->NameID.ToString();
	DAItem->DAItemInfo.AssetThumbnail = Item->AssetThumbnail;
	DAItem->DAItemInfo.bAvailableToInventory = Item->IsAvailableToInventory();
	DAItem->DAItemInfo.ItemType = Item->GetClass();
	DAItem->DAItemInfo.bIsBuidFromItinerary = true;

	return DAItem;
}
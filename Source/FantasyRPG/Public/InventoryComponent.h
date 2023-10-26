#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItem;
class UDAItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	bool InsertToInventory(AItem* Item);
	void ThrowFromItinerary();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int SSS = 23;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<const UDAItem*> InventoryItems;		
private:
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};

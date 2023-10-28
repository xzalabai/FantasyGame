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
	virtual void BeginPlay() override;
	bool InsertToInventory(AItem* Item);
	bool  RemoveFromInventory(const UDAItem* DAItem);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<const UDAItem*> InventoryItems;		
private:
	void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};

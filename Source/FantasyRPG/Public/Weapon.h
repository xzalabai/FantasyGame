#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponInterface.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USceneComponent;

UCLASS()
class FANTASYRPG_API AWeapon : public AItem, public IWeaponInterface
{
	GENERATED_BODY()
public:
	AWeapon();
	void EnableOverlappingEvents(bool Enable);
	FORCEINLINE UBoxComponent* GetOverlapArea() const { return OverlapArea; }
	UFUNCTION(BlueprintCallable)
	void PerformBoxTrace();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StartTrace;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndTrace;
	bool Picked = false;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* OverlapArea;
private:
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};

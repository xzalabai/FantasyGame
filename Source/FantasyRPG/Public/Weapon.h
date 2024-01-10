#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "EquipableInterface.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USceneComponent;
class UAnimInstance;
class AHeroCharacter;

UCLASS()
class FANTASYRPG_API AWeapon : public AItem, public IEquipableInterface
{
	GENERATED_BODY()
public:
	AWeapon();
	virtual void BeginPlay() override;
	virtual void EnableOverlappingEvents(bool bEnable) {};
	FORCEINLINE UBoxComponent* GetOverlapArea() const { return OverlapArea; }
	UFUNCTION(BlueprintCallable)
	void PerformBoxTrace();
	virtual void PerformMontage(class UAnimInstance *AnimInstance) override;
	virtual void AttackMontageStarted();
	virtual void AttackMontageEnded();
protected:
	UPROPERTY(EditDefaultsOnly)
	FName NameID = "";
	UPROPERTY(EditDefaultsOnly)
	UDataTable* WeaponDataTable;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StartTrace;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndTrace;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* OverlapArea;
	int Damage = 50;
	virtual void PerformActionOnNotify() override;
private:
	UPROPERTY()
	TArray<AActor*> AffectedActors;
};






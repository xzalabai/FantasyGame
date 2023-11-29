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
	virtual void EnableOverlappingEvents(bool bEnable) {};
	FORCEINLINE UBoxComponent* GetOverlapArea() const { return OverlapArea; }
	UFUNCTION(BlueprintCallable)
	void PerformBoxTrace();
	virtual void PerformMontage(class UAnimInstance *AnimInstance) override;
	virtual void AttackMontageStarted();
	virtual void AttackMontageEnded();
	virtual void OnItemEquipped(AHeroCharacter* MainCharacter) override;
	virtual void OnItemUnequipped() override;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StartTrace;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndTrace;
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* OverlapArea;
	UPROPERTY(EditDefaultsOnly)
	int Damage;
	virtual void PerformActionOnNotify() override;
private:
	UPROPERTY()
	TArray<AActor*> AffectedActors;
};






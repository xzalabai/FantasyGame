#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputControlComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class AHeroCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UInputControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInputControlComponent();

	// Input context -----------------------------------
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputContext;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ReleaseAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BlockAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InsertAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

	void TestM();

protected:
	virtual void BeginPlay() override;
private:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	friend class AHeroCharacter;
};

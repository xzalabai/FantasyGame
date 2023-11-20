#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectilePoolComponent.generated.h"

class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYRPG_API UProjectilePoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UProjectilePoolComponent();
	TObjectPtr<AProjectile> GetActorFromPool();
	void InsertToPool(TObjectPtr<AProjectile> Actor);
	void CreatePool(const TSubclassOf<AProjectile> ActorClass, int MaxSize);
protected:
	virtual void BeginPlay() override;
	
private:
	TObjectPtr<AProjectile> CreateActor();
	UPROPERTY()
	TArray<TObjectPtr<AProjectile>> Pool;
	TSubclassOf<AProjectile> ActorClass;
};

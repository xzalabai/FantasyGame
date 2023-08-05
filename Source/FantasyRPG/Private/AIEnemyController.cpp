#include "AIEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAIEnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void AAIEnemyController::Tick(float Dt)
{
    Super::Tick(Dt);
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
   // MoveToActor(PlayerPawn);
}



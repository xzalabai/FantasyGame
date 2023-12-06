#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "CharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HeroCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    
    StartTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Start Trace"));
    StartTrace ->SetupAttachment(RootComponent);
    
    EndTrace = CreateDefaultSubobject<USceneComponent>(TEXT("End Trace"));
    EndTrace->SetupAttachment(RootComponent);
}

void AWeapon::PerformBoxTrace()
{
    FHitResult OutHit;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    ActorsToIgnore.Add(GetAttachParentActor());
    bool bHit = UKismetSystemLibrary::BoxTraceSingle(
        this,
        StartTrace->GetComponentLocation(),
        EndTrace->GetComponentLocation(),
        FVector(3.3f,3.3f,3.3f),
        StartTrace->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore, EDrawDebugTrace::Persistent,
        OutHit,
        true);

    if (ICharacterInterface* ITarget = Cast<ICharacterInterface>(OutHit.GetActor()))
    {
        UE_LOG(LogTemp, Display, TEXT("[AWeapon] ITarget"));
        if (AffectedActors.Contains(OutHit.GetActor()))
        {
            UE_LOG(LogTemp, Display, TEXT("[AWeapon] AffectedActors"));
            // Actor is already in the list of affected
            return;
        }
        ITarget->OnReceivedHit(OutHit.ImpactPoint, OutHit.Location, GetAttachParentActor(), Damage);
        AffectedActors.Add(OutHit.GetActor());
    }
}

void AWeapon::PerformMontage(UAnimInstance *AnimInstance)
{
    AItem::PerformMontage(AnimInstance);
}

void AWeapon::AttackMontageStarted()
{
	
}

void AWeapon::AttackMontageEnded()
{
    UE_LOG(LogTemp, Display, TEXT("[AWeapon]AttackMontageEnded"));
	AffectedActors.Empty();
}

void AWeapon::PerformActionOnNotify()
{
	PerformBoxTrace();
}


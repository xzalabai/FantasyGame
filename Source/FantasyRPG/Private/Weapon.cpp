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
    OverlapArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    OverlapArea->SetupAttachment(RootComponent);
    
    StartTrace = CreateDefaultSubobject<USceneComponent>(TEXT("Start Trace"));
    StartTrace ->SetupAttachment(RootComponent);
    
    EndTrace = CreateDefaultSubobject<USceneComponent>(TEXT("End Trace"));
    EndTrace->SetupAttachment(RootComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
    OverlapArea->OnComponentBeginOverlap.AddUniqueDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (ItemState != EItemState::EIS_Equipped)
        return;
    return; // TODO
    // Perform hit
    FHitResult OutHit;
    PerformBoxTrace();

    // Inform Target about hit
    // done in performBoxTrace

    
    // Inform Owner about hit
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
        if (AffectedActors.Contains(OutHit.GetActor()))
        {
            // Actor is already in the list of affected
            return;
        }
        ITarget->OnReceivedHit(OutHit.ImpactPoint, OutHit.Location, GetAttachParentActor(), 50);
        AffectedActors.Add(OutHit.GetActor());
    }
}

void AWeapon::EnableOverlappingEvents(bool bEnable)
{
    // Overlapping events on the sword razor    
    OverlapArea->SetGenerateOverlapEvents(bEnable);
}

void AWeapon::PerformMontage(UAnimInstance *AnimInstance)
{
    AItem::PerformMontage(AnimInstance);
}

void AWeapon::AttackMontageStarted()
{
	EnableOverlappingEvents(true);
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


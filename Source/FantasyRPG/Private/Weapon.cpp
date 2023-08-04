// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "CharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"


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
    OverlapArea->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
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
    UKismetSystemLibrary::BoxTraceSingle(
        this,
        StartTrace->GetComponentLocation(),
        EndTrace->GetComponentLocation(),
        FVector(3.3f,3.3f,3.3f),
        StartTrace->GetComponentRotation(),
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore, EDrawDebugTrace::ForDuration,
        OutHit,
        true);

    if (ICharacterInterface* ITarget = Cast<ICharacterInterface>(OutHit.GetActor()))
    {
        ITarget->OnReceivedHit(OutHit.ImpactPoint);
    }
}

void AWeapon::EnableOverlappingEvents(bool Enable)
{
    // Overlapping events on the sword razor    
    OverlapArea->SetGenerateOverlapEvents(Enable);
}



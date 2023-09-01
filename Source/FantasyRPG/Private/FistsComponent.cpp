#include "FistsComponent.h"
#include "HeroCharacter.h"
#include "Components/BoxComponent.h"
#include "Fist.h"
#include "Item.h"
#include "DrawDebugHelpers.h"
#include "CharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HeroCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

UFistsComponent::UFistsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFistsComponent::BeginPlay()
{	
	Super::BeginPlay();

	LeftHand = GetWorld()->SpawnActor<AFist>(FistBPClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	RightHand = GetWorld()->SpawnActor<AFist>(FistBPClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	
	EnableOverlappingEvents(true);

}

void UFistsComponent::RegisterHandColliders()
{
	USkeletalMeshComponent* HeroCharacterMesh = GetHero()->GetMesh();
	RightHand->AttachToSocket(HeroCharacterMesh,"RightHandSocket");
	LeftHand->AttachToSocket(HeroCharacterMesh, "LeftHandSocket");
}

void UFistsComponent::EnableOverlappingEvents(bool bEnable)
{
	LeftHand->EnableOverlappingEvents(bEnable);
	RightHand->EnableOverlappingEvents(bEnable);
}

void UFistsComponent::PerformBoxTraceOnFists()
{
	LeftHand->PerformBoxTrace();
	RightHand->PerformBoxTrace();
}

void UFistsComponent::InitiateAttack(class AHeroCharacter &Character, class UAnimInstance &AnimInstance)
{
	UE_LOG(LogTemp, Display, TEXT("[UFistsComponent] InitiateAttack"));
    AnimInstance.Montage_Play(Montage);
	int32 RandomIndex = FMath::RandRange(0, AnimationSequenceName.Num() - 1);
	AnimInstance.Montage_JumpToSection(AnimationSequenceName[RandomIndex], Montage);
}

AHeroCharacter* UFistsComponent::GetHero()
{
	return Cast<AHeroCharacter>(GetOwner());
}

void UFistsComponent::AttackMontageStarted()
{
	EnableOverlappingEvents(true);
}

void UFistsComponent::AttackMontageEnded()
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "CharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet\KismetMathLibrary.h>

AGrenade::AGrenade()
{
	bAutoEquip = true;
	PrimaryActorTick.bCanEverTick = false;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->OnComponentHit.AddDynamic(this, &AGrenade::OnThrowableOverlap);
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrenade::Throw(FVector& Direction)
{
	UE_LOG(LogTemp, Display, TEXT("Throwing a grenade"));
	// detach from character
	const AHeroCharacter* Character = GetOwnerCharacter();
	Character->EquippedItem = nullptr;
	float ThrowForce = 1500.0f;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ItemState = EItemState::EIS_LayingOnGround;
		
	TogglePhysics(true);
    MeshComponent->AddImpulse(Direction * ThrowForce, "", true);
}

void AGrenade::OnItemEquipped(AHeroCharacter *MainCharacter)
{
	UE_LOG(LogTemp, Display, TEXT("Equipped Grenade"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Super::OnItemEquipped(MainCharacter);
}

void AGrenade::TogglePhysics(bool bEnable)
{
	if (bEnable)
	{
		TriggerCollider->SetGenerateOverlapEvents(true);
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetNotifyRigidBodyCollision(true);
	}
	else
	{
		TriggerCollider->SetGenerateOverlapEvents(true);
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AGrenade::OnThrowableOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("[AGrenade] Perform sphere trace and Destroy grenade "));
	PerformSphereTrace();
	Destroy();
}

void AGrenade::PerformMontage(class UAnimInstance *AnimInstance)
{
	AItem::PerformMontage(AnimInstance);
}

void AGrenade::EnableOverlappingEvents(bool Enable)
{
	return;
}

void AGrenade::PerformSphereTrace()
{
	FCollisionObjectQueryParams CollisionObjectTypes;
	CollisionObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

   	FCollisionShape MySphere = FCollisionShape::MakeSphere(200.0f);
	TArray<FHitResult> OutResults;
	GetWorld()->SweepMultiByObjectType(OutResults, GetActorLocation(), GetActorLocation(), FQuat::Identity, CollisionObjectTypes, MySphere);

	for (const FHitResult& OutHit : OutResults)
	{
		if (ICharacterInterface* ITarget = Cast<ICharacterInterface>(OutHit.GetActor()))
    	{
        	ITarget->OnReceivedHit(GetActorLocation(), nullptr, 50);
    	}
	}
}

void AGrenade::AttackMontageStarted()
{
	return;
}

void AGrenade::AttackMontageEnded()
{
	const AHeroCharacter* Character = GetOwnerCharacter();
	const float CharacterPitch = Character->GetCharacterPitch() / 100;
	FVector Direction = FVector(
		Character->GetActorForwardVector().X,
		Character->GetActorForwardVector().Y,
		Character->GetActorForwardVector().Z + CharacterPitch);
	Throw(Direction);
}

void AGrenade::PerformActionOnNotify()
{
	return;
}

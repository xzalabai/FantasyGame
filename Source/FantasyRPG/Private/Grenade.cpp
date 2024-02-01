// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "CharacterInterface.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetSystemLibrary.h"

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

void AGrenade::Throw(FVector& Direction)
{
	UE_LOG(LogTemp, Display, TEXT("Throwing a grenade"));
	
	// Detach from character
	const AHeroCharacter* Character = GetOwnerCharacter();
	Character->EquippedItem = nullptr;

	// Use a small cheat for caching the owner
	PreviousOwner = const_cast<AHeroCharacter*>(Character);
	
	// Throw
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	TogglePhysics(true);
	MeshComponent->AddImpulse(Direction * ThrowForce, "", true);
	//AStaticMeshActor* MeshA = Cast<AStaticMeshActor>(this);
	ItemState = EItemState::EIS_LayingOnGround;
}

void AGrenade::OnItemEquipped(AHeroCharacter *MainCharacter)
{
	UE_LOG(LogTemp, Display, TEXT("[AGrenade] Equipped Grenade"));
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

void AGrenade::PerformMontage(UAnimInstance* AnimInstance, FName MontageName, UAnimMontage* AnimMontage)
{
	AItem::PerformMontage(AnimInstance, MontageName, AnimMontage);
}

void AGrenade::PerformSphereTrace()
{
	FCollisionObjectQueryParams CollisionObjectTypes;
	CollisionObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	//CollisionObjectTypes.bIgnoreBlocks = false;
   	FCollisionShape MySphere = FCollisionShape::MakeSphere(ImpactArea);
	TArray<FHitResult> OutResults;
	DrawDebugSphere(GetWorld(), MeshComponent->GetComponentLocation(), ImpactArea, 100, FColor::Black, false, 20);
	GetWorld()->SweepMultiByObjectType(OutResults, MeshComponent->GetComponentLocation(), MeshComponent->GetComponentLocation(), FQuat::Identity, CollisionObjectTypes, MySphere);
	TArray<AActor*> ActorsHit;
	for (const FHitResult& OutHit : OutResults)
	{
		if (ActorsHit.Contains(OutHit.GetActor()))
		{
			continue;
		}
		ActorsHit.Add(OutHit.GetActor());

		UE_LOG(LogTemp, Display, TEXT("Hit -> %s"), *OutHit.GetComponent()->GetName());
		if (ICharacterInterface* ITarget = Cast<ICharacterInterface>(OutHit.GetActor()))
    	{
			FVector PoweredNormal = OutHit.ImpactNormal.GetSafeNormal() * 10;
        	ITarget->OnReceivedHit(PoweredNormal, OutHit.Location, PreviousOwner, 50);
    	}
	}
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

void AGrenade::AttackMontageStarted()
{
	return;
}

void AGrenade::EnableOverlappingEvents(bool Enable)
{
	return;
}
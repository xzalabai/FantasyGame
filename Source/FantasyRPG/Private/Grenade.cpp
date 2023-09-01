// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "CharacterInterface.h"
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

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrenade::Throw(FVector& Direction)
{
		UE_LOG(LogTemp, Warning, TEXT("Throwing a grenade"));
        float ThrowForce = 1500.0f;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ItemState = EItemState::EIS_LayingOnGround;
		TogglePhysics(true);
        MeshComponent->AddImpulse(Direction * ThrowForce, "", true);
}

void AGrenade::OnItemEquipped(AHeroCharacter &MainCharacter)
{
	UE_LOG(LogTemp, Display, TEXT("Equipped Grenade"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Equip();
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
	UE_LOG(LogTemp, Warning, TEXT("Your message"));
	PerformSphereTrace();
	Destroy();
}

void AGrenade::InitiateAttack(class AHeroCharacter &Character, class UAnimInstance &AnimInstance)
{
	AItem::InitiateAttack(Character, AnimInstance);
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
        	ITarget->OnReceivedHit(GetActorLocation());
    	}
	}
}

void AGrenade::AttackMontageStarted()
{
	
}

void AGrenade::AttackMontageEnded()
{
	
}


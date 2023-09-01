// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "NiagaraComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	TriggerCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Item Collider"));
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	
	SetRootComponent(MeshComponent);
	TriggerCollider->SetupAttachment(MeshComponent);
	ParticleSystem->SetupAttachment(RootComponent);

	
}

void AItem::AttachToSocket(USkeletalMeshComponent* PlayerMesh, FName SocketName)
{
	AttachToComponent(PlayerMesh, FAttachmentTransformRules::KeepWorldTransform, "RightHandSocket");
	FTransform SocketTransform = PlayerMesh->GetSocketTransform(SocketName);
	SetActorTransform(SocketTransform);
}

void AItem::Equip()
{
	ItemState = EItemState::EIS_Equipped;
	TriggerCollider->SetGenerateOverlapEvents(false);
}

void AItem::InitiateAttack(AHeroCharacter &Character, UAnimInstance &AnimInstance)
{
    UE_LOG(LogTemp, Display, TEXT("[AItem] InitiateAttack"));
    AnimInstance.Montage_Play(Montage);
    int32 RandomIndex = FMath::RandRange(0, AnimationSequenceName.Num() - 1);
	AnimInstance.Montage_JumpToSection(AnimationSequenceName[RandomIndex], Montage);
}

void AItem::Unequip()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ItemState = EItemState::EIS_LayingOnGround;
	TriggerCollider->SetGenerateOverlapEvents(true);

	// Find a suitabe position on ground
	FVector StartLocation = FVector(0.f, 0.f, 100.f);  	// Starting location of the raycast
	FVector EndLocation = FVector(0.f, 0.f, -1000.f);  	// Ending location of the raycast
	FHitResult HitResult; 							 	// Variable to store the hit result

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,           		// Hit result will be stored here
		GetActorLocation(),       	// Starting location of the raycast
		EndLocation,         		// Ending location of the raycast
		ECC_Visibility,      		// Collision channel (you can change it to suit your needs)
		FCollisionQueryParams::DefaultQueryParam,
		FCollisionResponseParams::DefaultResponseParam
	);
	if (bHit)
	{
		//SetActorRotation(FRotator(0,0,0));
		SetActorLocation(FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z + 20.0f));
	}
}
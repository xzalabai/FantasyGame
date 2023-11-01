// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DAItem.h"
#include "Animation/AnimInstance.h"
#include "NiagaraComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	TriggerCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Item Collider"));
	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	DAItem = CreateDefaultSubobject<UDAItem>(TEXT("DA Item"));
	
	SetRootComponent(MeshComponent);
	TriggerCollider->SetupAttachment(MeshComponent);
	ParticleSystem->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	// Replace item's mesh and characteristics
	if (IsAvailableToInventory() && !(DAItem->DAItemInfo.AssetName.IsEmpty()) && DAItem->DAItemInfo.AssetStaticMesh && DAItem->DAItemInfo.AssetThumbnail)
	{
		MeshComponent->SetStaticMesh(DAItem->DAItemInfo.AssetStaticMesh);
		bAvailableToInventory = DAItem->DAItemInfo.bAvailableToInventory;
	}	
}

void AItem::AttachToSocket(USkeletalMeshComponent* PlayerMesh, FName SocketName)
{
	AttachToComponent(PlayerMesh, FAttachmentTransformRules::KeepWorldTransform, "RightHandSocket");
	FTransform SocketTransform = PlayerMesh->GetSocketTransform(SocketName);
	SetActorTransform(SocketTransform);
}

void AItem::OnItemEquipped(AHeroCharacter &MainCharacter)
{
	
}

void AItem::Equip()
{
	ItemState = EItemState::EIS_Equipped;
	TriggerCollider->SetGenerateOverlapEvents(false);
}

void AItem::PerformMontage(UAnimInstance *AnimInstance)
{
	if (AnimationSequenceName.Num() == 0 || !Montage)
	{
		UE_LOG(LogTemp, Error, TEXT("[AItem] Item does not have a AnimMontage or MontageName set"));
		return;
	}
    int8 RandomIndex = FMath::RandRange(0, AnimationSequenceName.Num() - 1);
	PerformMontage(AnimInstance, AnimationSequenceName[RandomIndex], Montage);
}

void AItem::PerformMontage(UAnimInstance *AnimInstance, FName& MontageName, UAnimMontage* AnimMontage)
{
	UE_LOG(LogTemp, Display, TEXT("[AItem] PerformMontage"));
	AnimInstance->Montage_Play(AnimMontage);
	AnimInstance->Montage_JumpToSection(MontageName, AnimMontage);
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

const AHeroCharacter* AItem::GetOwnerCharacter() const
{
	AHeroCharacter* Character = Cast<AHeroCharacter>(GetAttachParentActor());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character not found"));
		return nullptr;
	}
	return Character;
}
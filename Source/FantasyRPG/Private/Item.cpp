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
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	SetRootComponent(RootSceneComponent);
	MeshComponent->SetupAttachment(RootSceneComponent);
	TriggerCollider->SetupAttachment(MeshComponent);
	ParticleSystem->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(DAItem) && DAItem->DAItemInfo.bIsBuidFromItinerary && !(DAItem->DAItemInfo.AssetName.IsEmpty()) && DAItem->DAItemInfo.AssetThumbnail)
	{
		AssetName = DAItem->DAItemInfo.AssetName;
		AssetThumbnail = DAItem->DAItemInfo.AssetThumbnail;
		bAvailableToInventory = DAItem->DAItemInfo.bAvailableToInventory;
	}
	else
	{

	}
}

void AItem::AttachToSocket(USkeletalMeshComponent* PlayerMesh, FName SocketName)
{
	UE_LOG(LogTemp, Error, TEXT("[AItem] AttachToSocket Item -> %s"), *this->GetName());
	AttachToComponent(PlayerMesh, FAttachmentTransformRules::KeepWorldTransform, "RightHandSocket");
	FTransform SocketTransform = PlayerMesh->GetSocketTransform(SocketName);
	SetActorTransform(SocketTransform);
}

void AItem::OnItemEquipped(AHeroCharacter *MainCharacter)
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


void AItem::OnItemUnequipped()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ItemState = EItemState::EIS_LayingOnGround;
	TriggerCollider->SetGenerateOverlapEvents(true);

	// Find a suitabe position on ground
	FVector EndLocation = FVector(0.f, 0.f, -1000.f);  	
	FHitResult HitResult; 							 	

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,           		// Hit result will be stored here
		GetActorLocation(),       	// Starting location of the raycast
		EndLocation,         		// Ending location of the raycast
		ECC_Camera,      			// Collision channel (you can change it to suit your needs)
		FCollisionQueryParams::DefaultQueryParam,
		FCollisionResponseParams::DefaultResponseParam
	);
	if (bHit)
	{
		SetActorLocation(FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z + 20.0f));
	}
}

const AHeroCharacter* AItem::GetOwnerCharacter() const
{
	AHeroCharacter* Character = Cast<AHeroCharacter>(GetAttachParentActor());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Character not found"));
		return nullptr;
	}
	return Character;
}
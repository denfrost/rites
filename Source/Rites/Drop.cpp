// Fill out your copyright notice in the Description page of Project Settings.

#include "Drop.h"
#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"

// Sets default values
ADrop::ADrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	MeshComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RootComponent = MeshComponent;
	ParticleComponent->SetupAttachment(MeshComponent);
	ParticleComponent->SecondsBeforeInactive = 0.0f;

	CreateNewItemOnBeginPlay = false;

	bReplicates = true;
}

void ADrop::SetItemData(FItemData Data)
{
	ensure(HasAuthority());
	ensure(Data.ItemClass == ItemClass);

	if (HasAuthority())
	{
		ItemData = Data;
		Item = CreateItem(Data);
	}
}

UItem* ADrop::GetItem()
{
	return Item;
}

UItem* ADrop::CreateItem(FItemData Data) const
{
	ensure(Data.ItemClass == ItemClass);
	UItem* ReturnItem = NewObject<UItem>(GetTransientPackage(), ItemClass);

	ensure(ReturnItem != nullptr);

	if (ReturnItem != nullptr)
	{
		ReturnItem->SetItemData(Data);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Failed to create item"));
	}

	return ReturnItem;
}

void ADrop::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADrop, ItemData);
}

// Called when the game starts or when spawned
void ADrop::BeginPlay()
{
	Super::BeginPlay();

	ensure(ItemClass.Get() != nullptr);

	if (CreateNewItemOnBeginPlay && HasAuthority())
	{	
		Item = UItem::CreateNewItem(ItemClass);
		ItemData = Item->GetItemData();
	}

	// Start particle system
	ParticleComponent->Activate();

	const float DropCastDistance = 50000.0f;
	
	// Attempt to move drop down until it hits a world static object.
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - DropCastDistance * FVector::UpVector, ECC_WorldStatic /*, ECC_WorldStatic*/);

	if (HitResult.bBlockingHit)
	{
		SetActorLocation(HitResult.Location);
	}
}

void ADrop::OnRep_ItemData()
{
	ensure(!HasAuthority());
	Item = CreateItem(ItemData);
}
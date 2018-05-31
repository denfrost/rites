// Fill out your copyright notice in the Description page of Project Settings.

#include "Drop.h"
#include "Item.h"
#include "Gear.h"
#include "Gem.h"

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

void ADrop::TransferItemToDrop(UItem* NewItem)
{
	ensure(HasAuthority());

	if (HasAuthority())
	{
		Item = NewItem;

		// Clear and Rewrite DropData (for replication)
		DropData = FDropData();

		if (Item != nullptr)
		{
			DropData.BaseItemData = NewItem->GetItemData();

			UGear* Gear = Cast<UGear>(Item);

			if (Gear != nullptr)
			{
				// Write data for socketed gem 1 
				if (Gear->GetSockets().Num() >= 1 &&
					Gear->GetSockets()[0].Gem != nullptr)
				{
					DropData.Gem0ItemData = Gear->GetSockets()[0].Gem->GetItemData();
				}

				if (Gear->GetSockets().Num() >= 2 &&
					Gear->GetSockets()[1].Gem != nullptr)
				{
					DropData.Gem1ItemData = Gear->GetSockets()[1].Gem->GetItemData();
				}
			}
		}
	}
}

UItem* ADrop::GetItem()
{
	return Item;
}

UItem* ADrop::CreateItem(FItemData Data) const
{
	UItem* ReturnItem = NewObject<UItem>(GetTransientPackage(), Data.ItemClass);

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

void ADrop::CreateAndSocketGem(FItemData GemData, int32 SocketIndex)
{
	UGear* Gear = Cast<UGear>(Item);

	// Only create and socket gem if this base item is a gear and 
	// the gem data has a valid item.
	if (Gear != nullptr &&
		GemData.ItemClass != nullptr &&
		GemData.InstanceID != 0)
	{
		// Instantiate and socket Gem 1.
		ensure(Gear->GetSockets().Num() >= SocketIndex + 1);

		if (Gear->GetSockets().Num() >= SocketIndex + 1)
		{
			Gear->GetSockets()[SocketIndex].Gem = Cast<UGem>(CreateItem(GemData));
			ensure(Gear->GetSockets()[SocketIndex].Gem != nullptr);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Drop could not attach Gem to Gear because it is missing a socket."));
		}
	}
}

void ADrop::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADrop, DropData);
}

// Called when the game starts or when spawned
void ADrop::BeginPlay()
{
	Super::BeginPlay();

	if (CreateNewItemOnBeginPlay && HasAuthority())
	{	
		Item = UItem::CreateNewItem(SpawnItemClass);
		DropData.BaseItemData = Item->GetItemData();
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

void ADrop::OnRep_DropData()
{
	ensure(!HasAuthority());

	Item = CreateItem(DropData.BaseItemData);
	ensure(Item != nullptr);

	if (Item != nullptr)
	{
		CreateAndSocketGem(DropData.Gem0ItemData, 0);
		CreateAndSocketGem(DropData.Gem1ItemData, 1);
	}
}
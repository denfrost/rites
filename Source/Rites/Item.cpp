// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Drop.h"

#include "Engine/World.h"

int32 UItem::LastInstanceID = 0;

UItem::UItem()
{
	Name = "Unknown Item";
	StackSize = 1;
	Count = 1;
	Durability = 100;
}

UItem* UItem::CreateNewItem(TSubclassOf<UItem> ItemClass)
{
	ensure(ItemClass.Get() != nullptr);
	
	UItem* NewItem = NewObject<UItem>(GetTransientPackage(), ItemClass.Get());
	ensure(NewItem != nullptr);

	NewItem->InstanceID = ++LastInstanceID;

	return NewItem;
}

ADrop* UItem::SpawnDrop(FVector Location) const
{
	ADrop* ReturnDrop = nullptr;

	ensure(GIsServer);
	if (GIsServer)
	{
		ReturnDrop = Cast<ADrop>(GetWorld()->SpawnActor(DropClass.Get(), &Location));
	}

	return ReturnDrop;
}

UTexture2D* UItem::GetTexture()
{
	return InterfaceTexture;
}

FString& UItem::GetName()
{
	return Name;
}

int32 UItem::GetInstanceID()
{
	return InstanceID;
}

TSubclassOf<ADrop> UItem::GetDropClass()
{
	return DropClass;
}

FItemData UItem::GetItemData()
{
	FItemData ReturnData;
	ReturnData.ItemClass = GetClass();
	ReturnData.InstanceID = InstanceID;
	ReturnData.Durability = Durability;
	ReturnData.Count = Count;

	return ReturnData;
}

void UItem::SetItemData(FItemData Data)
{
	ensure(Data.ItemClass == GetClass());

	InstanceID = Data.InstanceID;
	Durability = Data.Durability;
	Count = Data.Count;
}
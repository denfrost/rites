// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Drop.h"

#include "Engine/World.h"

UItem::UItem()
{

}

ADrop* UItem::SpawnDrop(FVector Location) const
{
	return Cast<ADrop>(GetWorld()->SpawnActor(DropClass.Get(), &Location));
}
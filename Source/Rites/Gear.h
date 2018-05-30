// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Enums.h"
#include "GemSocket.h"
#include "Gear.generated.h"


UCLASS()
class RITES_API UGear : public UItem
{
	GENERATED_BODY()
	
public:


protected:

	UPROPERTY(EditAnywhere)
	TArray<FGemSocket> Sockets;

	UPROPERTY(EditAnywhere)
	int32 Armor;
	
	UPROPERTY(EditAnywhere)
	EGearSlotType SlotType;
};

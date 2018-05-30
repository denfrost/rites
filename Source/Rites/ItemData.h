// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:

	FItemData();

	UPROPERTY(EditAnywhere)
	UClass* ItemClass;

	UPROPERTY(EditAnywhere)
	int32 InstanceID;

	UPROPERTY(EditAnywhere)
	int32 Count;

	UPROPERTY(EditAnywhere)
	int32 Durability;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture.h"
#include "Item.generated.h"

class ADrop;

UCLASS()
class RITES_API UItem : public UObject
{
	GENERATED_BODY()
	
public:

	UItem();

	ADrop* SpawnDrop(FVector Location) const;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADrop> DropClass;

	UPROPERTY(EditAnywhere)
	UTexture2D* InterfaceTexture;

	UPROPERTY(EditAnywhere)
	int32 StackSize;

	UPROPERTY(EditAnywhere)
	int32 Count;
	
};

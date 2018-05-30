// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture.h"
#include "ItemData.h"
#include "Item.generated.h"

class ADrop;

UCLASS(Blueprintable)
class RITES_API UItem : public UObject
{
	GENERATED_BODY()
	
public:

	UItem();

	static UItem* CreateNewItem(TSubclassOf<UItem> ItemClass);

	ADrop* SpawnDrop(FVector Location) const;

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetTexture();

	UFUNCTION(BlueprintCallable)
	FString& GetName();

	UFUNCTION(BlueprintCallable)
	int32 GetInstanceID();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ADrop> GetDropClass();

	UFUNCTION(BlueprintCallable)
	FItemData GetItemData();

	UFUNCTION(BlueprintCallable)
	void SetItemData(FItemData Data);

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADrop> DropClass;

	UPROPERTY(EditAnywhere)
	UTexture2D* InterfaceTexture;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(VisibleAnywhere)
	int32 InstanceID;

	UPROPERTY(EditAnywhere)
	int32 StackSize;

	UPROPERTY(EditAnywhere)
	int32 Count;

	UPROPERTY(EditAnywhere)
	int32 Durability;

	static int32 LastInstanceID;
	
};

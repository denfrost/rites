// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Drop.generated.h"

class UParticleSystemComponent;
class UItem;

USTRUCT(BlueprintType)
struct FDropData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FItemData BaseItemData;

	UPROPERTY(EditAnywhere)
	FItemData Gem0ItemData;

	UPROPERTY(EditAnywhere)
	FItemData Gem1ItemData;
};

UCLASS()
class RITES_API ADrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrop();

	void TransferItemToDrop(UItem* NewItem);

	UFUNCTION(BlueprintCallable)
	UItem* GetItem();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UItem* CreateItem(FItemData Data) const;

	void CreateAndSocketGem(FItemData GemData, int32 SocketIndex);

	UFUNCTION()
	void OnRep_DropData();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItem> SpawnItemClass;

	UPROPERTY(VisibleAnywhere)
	UItem* Item;

	UPROPERTY(ReplicatedUsing = OnRep_DropData)
	FDropData DropData;

	UPROPERTY(EditAnywhere)
	bool CreateNewItemOnBeginPlay;
};

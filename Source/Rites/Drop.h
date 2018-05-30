// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Drop.generated.h"

class UParticleSystemComponent;
class UItem;

UCLASS()
class RITES_API ADrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrop();

	void SetItemData(FItemData Data);

	UFUNCTION(BlueprintCallable)
	UItem* GetItem();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UItem* CreateItem(FItemData Data) const;

	UFUNCTION()
	void OnRep_ItemData();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItem> ItemClass;

	UPROPERTY(VisibleAnywhere)
	UItem* Item;

	UPROPERTY(ReplicatedUsing = OnRep_ItemData)
	FItemData ItemData;

	UPROPERTY(EditAnywhere)
	bool CreateNewItemOnBeginPlay;
};

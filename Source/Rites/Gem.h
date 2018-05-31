// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Enums.h"
#include "Gem.generated.h"

class AAbility;
class AFighter;

UCLASS()
class RITES_API UGem : public UItem
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	EGemColor GetColor() const;

	UFUNCTION(BlueprintCallable)
	float GetCooldownTime() const;

	UFUNCTION(BlueprintCallable)
	float GetCooldownRemaining() const;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxChargers() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCharges() const;

	UFUNCTION(BlueprintNativeEvent)
	void Activate(AFighter* Fighter);

	UFUNCTION(BlueprintNativeEvent)
	void Release(AFighter* Fighter);

	UFUNCTION(BlueprintNativeEvent)
	void Tick(AFighter* Fighter, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEquip(AFighter* Fighter);

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip(AFighter* Fighter);

	UFUNCTION(BlueprintNativeEvent)
	void OnDamaged(AFighter* Fighter);

protected:

	UPROPERTY(EditAnywhere)
	EGemColor Color;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<AAbility> AbilityClass;

	UPROPERTY(EditAnywhere)
	float CooldownTime;

	UPROPERTY(EditAnywhere)
	float CooldownRemaining;

	UPROPERTY(EditAnywhere)
	int32 MaxCharges;

	UPROPERTY(EditAnywhere)
	int32 Charges;
};

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

	UGem();

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

	UFUNCTION(BlueprintCallable)
	float GetGlobalCooldownTime() const;

	UFUNCTION(BlueprintCallable)
	bool IsChanneled() const;

	UFUNCTION(BlueprintCallable)
	bool IsCurrentlyBeingChanneled() const;

	UFUNCTION(BlueprintCallable)
	bool IsOnCooldown() const;

	UFUNCTION(BlueprintCallable)
	bool IsPassive() const;

	virtual void Activate(AFighter* Fighter);

	virtual void Release(AFighter* Fighter);

	virtual void Tick(AFighter* Fighter, float DeltaTime);

	virtual void OnEquip(AFighter* Fighter);

	virtual void OnUnequip(AFighter* Fighter);

	virtual void OnDamaged(AFighter* Fighter);

	void RegisterActivate(AFighter* Fighter);

	void RegisterRelease(AFighter* Fighter);

protected:

	UPROPERTY(EditAnywhere)
	EGemColor Color;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAbility> AbilityClass;

	UPROPERTY(EditAnywhere)
	float CooldownTime;

	UPROPERTY(EditAnywhere)
	float CooldownRemaining;

	UPROPERTY(EditAnywhere)
	int32 MaxCharges;

	UPROPERTY(EditAnywhere)
	int32 Charges;

	UPROPERTY(EditAnywhere)
	float GlobalCooldownTime;

	UPROPERTY(EditAnywhere)
	bool bChanneled;

	UPROPERTY(EditAnywhere)
	bool bPassive;

	UPROPERTY(VisibleAnywhere)
	bool bCurrentlyBeingChanneled;
};

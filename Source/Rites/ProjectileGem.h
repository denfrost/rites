// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gem.h"
#include "ProjectileGem.generated.h"

UCLASS()
class RITES_API UProjectileGem : public UGem
{
	GENERATED_BODY()
	
public:

	UProjectileGem();
	
	virtual void Activate(AFighter* Fighter) override;

	virtual void Release(AFighter* Fighter) override;

	virtual void Tick(AFighter* Fighter, float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere)
	int32 BurstCount;

	UPROPERTY(EditAnywhere)
	float SpreadAngle;

	UPROPERTY(EditAnywhere)
	float AutoFireDelay;

	UPROPERTY(EditAnywhere)
	FRotator AimOffset;
};

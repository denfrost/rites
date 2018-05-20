// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterStats.generated.h"

USTRUCT(BlueprintType)
struct FFighterStats
{
	GENERATED_BODY()
	
public:

	FFighterStats()
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvulnerable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFlying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSlowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMuted;
	
};

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
		Health = 100;
		MaxHealth = Health;
		MoveSpeed = 400.0f;
		JumpSpeed = 550.0f;
		ReloadSpeed = 2.0f;
		bInvulnerable = false;
		bInvisible = false;
		bFlying = false;
		bStunned = false;
		bSlowed = false;
		bMuted = false;
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

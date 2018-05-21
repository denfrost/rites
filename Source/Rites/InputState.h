// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputState.generated.h"

USTRUCT(BlueprintType)
struct FInputState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FVector2D MoveDirection;

	UPROPERTY(BlueprintReadWrite)
	FVector2D RotateDirection;

	UPROPERTY(BlueprintReadWrite)
	bool bJumpDown;

	UPROPERTY(BlueprintReadWrite)
	bool bCastLeftDown;

	UPROPERTY(BlueprintReadWrite)
	bool bCastRightDown;

	UPROPERTY(BlueprintReadWrite)
	bool bRechargeDown;

	UPROPERTY(BlueprintReadWrite)
	bool bActivateDown;

};

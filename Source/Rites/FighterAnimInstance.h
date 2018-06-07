// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums.h"
#include "FighterAnimInstance.generated.h"

class AFighter;

/**
 * 
 */
UCLASS()
class RITES_API UFighterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	EFighterBaseAnimationState BaseAnimationState;

	UPROPERTY(BlueprintReadWrite)
	EFighterOverrideAnimationState OverrideAnimationState;

	UPROPERTY(BlueprintReadWrite)
	float PlayRate;

	UPROPERTY(BlueprintReadWrite)
	float LookAngle;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D InputDirection;

	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite)
	bool bJumping;

	UPROPERTY(BlueprintReadWrite)
	bool bGrounded;

	UPROPERTY(BlueprintReadWrite)
	bool bCastLeft;

	UPROPERTY(BlueprintReadWrite)
	bool bCastRight;

	UPROPERTY(BlueprintReadWrite)
	AFighter* Fighter;
};

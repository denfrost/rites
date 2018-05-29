// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

UENUM(BlueprintType)
enum class EFighterBaseAnimationState : uint8
{
	None,
	Idle,
	RunForward,
	RunStrafe,
	RunBackwards,
	Jump,
	Fall
};

UENUM(BlueprintType)
enum class EFighterOverrideAnimationState : uint8
{
	None,
	CastLeft,
	CastRight,
	Recharge
};

UENUM(BlueprintType)
enum class EGemColor : uint8
{
	Red,
	Green,
	Blue
};

UENUM(BlueprintType)
enum class EEquipSlots : uint8
{
	Head,
	Chest,
	Legs,
	LeftGlove,
	RightGlove

};
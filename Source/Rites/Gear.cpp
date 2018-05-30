// Fill out your copyright notice in the Description page of Project Settings.

#include "Gear.h"

TArray<FGemSocket>& UGear::GetSockets()
{
	return Sockets;
}

int32 UGear::GetArmor()
{
	return Armor;
}

EGearType UGear::GetType()
{
	return Type;
}


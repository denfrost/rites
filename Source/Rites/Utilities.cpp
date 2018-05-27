// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities.h"

float Utils::FindAngleBetweenVectors(FVector v1, FVector v2)
{
	v1.Normalize();
	v2.Normalize();

	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(v1, v2)));
}
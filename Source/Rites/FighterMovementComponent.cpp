// Fill out your copyright notice in the Description page of Project Settings.
#include "FighterMovementComponent.h"
#include "Utilities.h"
#include "Constants.h"

void UFighterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFighterMovementComponent::Move(float fDeltaTime, FVector Velocity)
{
	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(fDeltaTime))
	{
		return;
	}

	// Get (and then clear) the movement vector
	if (!Velocity.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Velocity * fDeltaTime, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(Velocity * fDeltaTime, 1.0f - Hit.Time, Hit.Normal, Hit);
		}
	}
}

bool UFighterMovementComponent::MoveVert(float fDeltaTime, float fVertSpeed, bool& bUpwardCollision)
{
	// Set flag to false for no upward collision
	bUpwardCollision = false;

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(fDeltaTime))
	{
		return false;
	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, fVertSpeed * fDeltaTime), UpdatedComponent->GetComponentRotation(), true, Hit);

	// If component hit something while moving downward, check if grounded.
	if (Hit.IsValidBlockingHit() &&
		fVertSpeed <= 0.0f)
	{
		// Save the initial normal first, as the Hit object is changed in Slide function call
		float fIncline = Utils::FindAngleBetweenVectors(Hit.ImpactNormal, FVector(0.0f, 0.0f, 1.0f));

		// Slide the object on landing
		SlideAlongSurface(FVector(0.0f, 0.0f, fVertSpeed * fDeltaTime), 1.0f - Hit.Time, Hit.Normal, Hit);

		// It hit something, but only return grounded as true if the incline isn't steep
		if (fIncline < Constants::MaxGroundingInclineAngle)
		{
			return true;
		}
	}
	else if (Hit.IsValidBlockingHit() &&
		fVertSpeed > 0.0f)
	{
		bUpwardCollision = true;
	}

	return false;
}
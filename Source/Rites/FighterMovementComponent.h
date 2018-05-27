// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "FighterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class RITES_API UFighterMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:

	void Move(float DeltaTime, FVector Velocity);

	bool MoveHeroVert(float DeltaTime, float VertSpeed, int& nUpwardCollision);

protected:

	UPROPERTY(EditAnywhere)
	float MaxGroundingInclineAngle = 45.0f;
	
};

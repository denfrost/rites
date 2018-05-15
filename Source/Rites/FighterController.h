// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FighterController.generated.h"

/**
 * 
 */
UCLASS()
class RITES_API AFighterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AFighterController();

	void SetupInputComponent() override;

protected:

	// Input handlers
	void JumpPressed();
	void JumpReleased();
	void CastLeftPressed();
	void CastLeftReleased();
	void CastRightPressed();
	void CastRightReleased();
	void InventoryPressed();
	void OptionsPressed();
	void RechargePressed();
	void RechargeReleased();
	void ActivatePressed();
	void ActivateReleased();
	void MapPressed();
	void RotateHorizontal(float AxisValue);
	void RotateVertical(float AxisValue);
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

};

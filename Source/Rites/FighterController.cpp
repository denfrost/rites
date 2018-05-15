// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterController.h"

AFighterController::AFighterController()
{

}

void AFighterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	if (InputComponent != NULL)
	{
		// Action bindings
		InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AFighterController::JumpPressed);
		InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AFighterController::JumpReleased);
		InputComponent->BindAction("CastLeft", EInputEvent::IE_Pressed, this, &AFighterController::CastLeftPressed);
		InputComponent->BindAction("CastLeft", EInputEvent::IE_Released, this, &AFighterController::CastLeftReleased);
		InputComponent->BindAction("CastRight", EInputEvent::IE_Pressed, this, &AFighterController::CastRightPressed);
		InputComponent->BindAction("CastRight", EInputEvent::IE_Released, this, &AFighterController::CastRightReleased);
		InputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &AFighterController::InventoryPressed);
		InputComponent->BindAction("Options", EInputEvent::IE_Pressed, this, &AFighterController::OptionsPressed);
		InputComponent->BindAction("Recharge", EInputEvent::IE_Pressed, this, &AFighterController::RechargePressed);
		InputComponent->BindAction("Recharge", EInputEvent::IE_Released, this, &AFighterController::RechargeReleased);
		InputComponent->BindAction("Activate", EInputEvent::IE_Pressed, this, &AFighterController::ActivatePressed);
		InputComponent->BindAction("Activate", EInputEvent::IE_Released, this, &AFighterController::ActivateReleased);
		InputComponent->BindAction("Map", EInputEvent::IE_Pressed, this, &AFighterController::MapPressed);

		// Axis bindings
		InputComponent->BindAxis("RotateHorizontal", this, &AFighterController::RotateHorizontal);
		InputComponent->BindAxis("RotateVertical", this, &AFighterController::RotateVertical);
		InputComponent->BindAxis("MoveForward", this, &AFighterController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AFighterController::MoveRight);
	}
}

void AFighterController::JumpPressed()
{

}

void AFighterController::JumpReleased()
{

}
void AFighterController::CastLeftPressed()
{

}

void AFighterController::CastLeftReleased()
{

}

void AFighterController::CastRightPressed()
{

}

void AFighterController::CastRightReleased()
{

}

void AFighterController::InventoryPressed()
{

}

void AFighterController::OptionsPressed()
{

}

void AFighterController::RechargePressed()
{

}

void AFighterController::RechargeReleased()
{

}

void AFighterController::ActivatePressed()
{

}

void AFighterController::ActivateReleased()
{

}

void AFighterController::MapPressed()
{

}

void AFighterController::RotateHorizontal(float AxisValue)
{
	AxisValue;
}

void AFighterController::RotateVertical(float AxisValue)
{
	AxisValue;
}

void AFighterController::MoveForward(float AxisValue)
{
	AxisValue;
}

void AFighterController::MoveRight(float AxisValue)
{
	AxisValue;
}

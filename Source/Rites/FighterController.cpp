// Fill out your copyright notice in the Description page of Project Settings.

#include "FighterController.h"
#include "RitesGameInstance.h"

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

void AFighterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	URitesGameInstance* RitesGameInstance = Cast<URitesGameInstance>(GetGameInstance());

	if (RitesGameInstance != nullptr)
	{
		if (InPawn != nullptr &&
			!RitesGameInstance->IsMenuOpen("HUD"))
		{
			RitesGameInstance->OpenMenu("HUD");
		}
		else if (InPawn == nullptr &&
			RitesGameInstance->IsMenuOpen("HUD"))
		{
			RitesGameInstance->CloseMenu("HUD");
		}
	}
}

const FInputState& AFighterController::GetInputState() const
{
	return InputState;
}

void AFighterController::JumpPressed()
{
	InputState.bJumpDown = true;
}

void AFighterController::JumpReleased()
{
	InputState.bJumpDown = false;
}
void AFighterController::CastLeftPressed()
{
	InputState.bCastLeftDown = true;
}

void AFighterController::CastLeftReleased()
{
	InputState.bCastLeftDown = false;
}

void AFighterController::CastRightPressed()
{
	InputState.bCastRightDown = true;
}

void AFighterController::CastRightReleased()
{
	InputState.bCastRightDown = false;
}

void AFighterController::InventoryPressed()
{
	URitesGameInstance* GameInstance = Cast<URitesGameInstance>(GetGameInstance());
	ensure(GameInstance != nullptr);

	GameInstance->ToggleMenu(TEXT("Inventory"));

	if (GameInstance->IsMenuOpen("Inventory"))
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AFighterController::OptionsPressed()
{

}

void AFighterController::RechargePressed()
{
	InputState.bRechargeDown = true;
}

void AFighterController::RechargeReleased()
{
	InputState.bRechargeDown = false;
}

void AFighterController::ActivatePressed()
{
	InputState.bActivateDown = true;
}

void AFighterController::ActivateReleased()
{
	InputState.bActivateDown = true;
}

void AFighterController::MapPressed()
{

}

void AFighterController::RotateHorizontal(float AxisValue)
{
	InputState.RotateDirection.X = AxisValue;
}

void AFighterController::RotateVertical(float AxisValue)
{
	InputState.RotateDirection.Y = AxisValue;
}

void AFighterController::MoveForward(float AxisValue)
{
	InputState.MoveDirection.Y = AxisValue;
}

void AFighterController::MoveRight(float AxisValue)
{
	InputState.MoveDirection.X = AxisValue;
}

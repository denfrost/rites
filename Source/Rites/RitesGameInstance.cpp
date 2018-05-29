// Fill out your copyright notice in the Description page of Project Settings.

#include "RitesGameInstance.h"

URitesGameInstance::URitesGameInstance()
{

}

void URitesGameInstance::ToggleMenu(const FString& Name)
{
	if (!IsMenuOpen(Name))
	{
		OpenMenu(TEXT("Inventory"));
	}
	else
	{
		CloseMenu(TEXT("Inventory"));
	}
}

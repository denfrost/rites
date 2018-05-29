// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "RitesGameInstance.generated.h"

UCLASS()
class RITES_API URitesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	URitesGameInstance();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenMenu(const FString& Name);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseMenu(const FString& Name);

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, BlueprintCallable)
	bool IsMenuOpen(const FString& Name);

	UFUNCTION(BlueprintCallable)
	void ToggleMenu(const FString& Name);
	
};

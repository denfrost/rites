// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameMode.generated.h"

class AFighter;

UCLASS()
class RITES_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void OnFighterKilled(AFighter* Fighter);

protected:

	virtual void BeginPlay() override;
	
	//TArray<APlayerStart*> PlayerStarts;

};

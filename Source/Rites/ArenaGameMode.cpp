// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaGameMode.h"
#include "Fighter.h"

#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

void AArenaGameMode::BeginPlay()
{
	//for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	APlayerStart* PlayerStart = *ActorItr;
	//}
}

void AArenaGameMode::OnFighterKilled(AFighter* Fighter)
{
	ensure(Fighter != nullptr);

	if (Fighter != nullptr)
	{
		// Drop all items
		Fighter->DropAllItems();

		// Reset Fighter stats
		Fighter->ResetStats();

		// Move Fighter to player start
		AActor* PlayerStart = FindPlayerStart(Fighter->GetController());

		ensure(PlayerStart != nullptr);
		if (PlayerStart != nullptr)
		{
			Fighter->Transport(PlayerStart->GetActorLocation());
		}
	}
}
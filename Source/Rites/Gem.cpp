// Fill out your copyright notice in the Description page of Project Settings.

#include "Gem.h"
#include "Fighter.h"
#include "Ability.h"
#include "Engine/Engine.h"

UGem::UGem()
{
	Color = EGemColor::Red;
	AbilityClass = nullptr;
	CooldownTime = 2.0f;
	CooldownRemaining = 0.0f;
	MaxCharges = 0;
	Charges = 0;
	GlobalCooldownTime = 0.5f;
	bChanneled = false;
	bPassive = false;
	bCurrentlyBeingChanneled = false;
}

EGemColor UGem::GetColor() const
{
	return Color;
}

float UGem::GetCooldownTime() const
{
	return CooldownTime;
}

float UGem::GetCooldownRemaining() const
{
	return CooldownRemaining;
}

int32 UGem::GetMaxChargers() const
{
	return MaxCharges;
}

int32 UGem::GetCharges() const
{
	return Charges;
}

float UGem::GetGlobalCooldownTime() const
{
	return GlobalCooldownTime;
}

bool UGem::IsChanneled() const
{
	return bChanneled;
}

bool UGem::IsCurrentlyBeingChanneled() const
{
	return bCurrentlyBeingChanneled;
}

bool UGem::IsOnCooldown() const
{
	// TODO: If client-side activation gets implemented (for things like projectiles), then
	// we might have to store a new boolean that gets updated by a message from the server when the 
	// server registers that the cooldown has finished. Or, we have a message that gets sent back to the client
	// when an activation message fails, and this will reset the cooldownremainingtime on the gem for the client.
	// This will probably produce less network traffic.
	return CooldownRemaining > 0.0f;
}

bool UGem::IsPassive() const
{
	return bPassive;
}

void UGem::Activate(AFighter* Fighter)
{
	ensure(Fighter->HasAuthority());
	RegisterActivate(Fighter);

	ensure(AbilityClass != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Gem Activated!"));

	if (AbilityClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Ability Spawned!"));
		FVector SpawnLocation = Fighter->GetActorLocation();
		AAbility* SpawnedAbility = Cast<AAbility>(Fighter->GetWorld()->SpawnActor(AbilityClass.Get(), &SpawnLocation));
		
		ensure(SpawnedAbility != nullptr);
		SpawnedAbility->SetCaster(Fighter);

		SpawnedAbility->EnableOverlaps(true);
	}
}

void UGem::Release(AFighter* Fighter)
{
	ensure(Fighter->HasAuthority());
}

void UGem::Tick(AFighter* Fighter, float DeltaTime)
{
	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining -= DeltaTime;
	}
}

void UGem::OnEquip(AFighter* Fighter)
{
	ensure(Fighter->HasAuthority());
}

void UGem::OnUnequip(AFighter* Fighter)
{
	ensure(Fighter->HasAuthority());
}

void UGem::OnDamaged(AFighter* Fighter)
{
	ensure(Fighter->HasAuthority());
}

void UGem::RegisterActivate(AFighter* Fighter)
{
	if (bChanneled)
	{
		bCurrentlyBeingChanneled = true;
	}
	else
	{
		CooldownRemaining = CooldownTime;
		Fighter->SetGlobalCooldown(GlobalCooldownTime);
	}

}

void UGem::RegisterRelease(AFighter* Fighter)
{
	if (bChanneled)
	{
		bCurrentlyBeingChanneled = false;
		CooldownRemaining = CooldownTime;
		Fighter->SetGlobalCooldown(GlobalCooldownTime);
	}
}

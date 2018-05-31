// Fill out your copyright notice in the Description page of Project Settings.

#include "Gem.h"
#include "Fighter.h"

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

void UGem::Activate_Implementation(AFighter* Fighter)
{
	if (Fighter->HasAuthority())
	{
		
	}
}

void UGem::Release_Implementation(AFighter* Fighter)
{

}

void UGem::Tick_Implementation(AFighter* Fighter, float DeltaTime)
{

}

void UGem::OnEquip_Implementation(AFighter* Fighter)
{

}

void UGem::OnUnequip_Implementation(AFighter* Fighter)
{

}

void UGem::OnDamaged_Implementation(AFighter* Fighter)
{

}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Gem.h"
#include "Fighter.h"

void UGem::Activate_Implementation(AFighter* Fighter)
{
	if (Fighter->HasAuthority())
	{
		
	}
}

UFUNCTION(BlueprintNativeEvent)
void UGem::Release_Implementation(AFighter* Fighter)
{

}

UFUNCTION(BlueprintNativeEvent)
void UGem::Tick_Implementation(AFighter* Fighter, float DeltaTime)
{

}

UFUNCTION(BlueprintNativeEvent)
void UGem::OnEquip_Implementation(AFighter* Fighter)
{

}

UFUNCTION(BlueprintNativeEvent)
void UGem::OnUnequip_Implementation(AFighter* Fighter)
{

}

UFUNCTION(BlueprintNativeEvent)
void UGem::OnDamaged_Implementation(AFighter* Fighter)
{

}
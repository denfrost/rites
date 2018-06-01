// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileGem.h"

UProjectileGem::UProjectileGem()
{

}

void UProjectileGem::Activate(AFighter* Fighter)
{
	Super::Activate(Fighter);
}

void UProjectileGem::Release(AFighter* Fighter)
{
	Super::Release(Fighter);
}

void UProjectileGem::Tick(AFighter* Fighter, float DeltaTime)
{
	Super::Tick(Fighter, DeltaTime);
}
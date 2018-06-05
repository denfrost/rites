// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileGem.h"
#include "Fighter.h"
#include "ProjectileAbility.h"
#include "Engine/Engine.h"

UProjectileGem::UProjectileGem()
{
	BurstCount = 1;
	SpreadAngle = 0.0f;
	AutoFireDelay = 0.0f;
}

void UProjectileGem::Activate(AFighter* Fighter)
{
	ensure(Fighter->HasAuthority());
	RegisterActivate(Fighter);

	ensure(AbilityClass != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Projectile Gem Activated!"));

	if (AbilityClass != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Projectile Ability Spawned!"));
		FVector SpawnLocation = Fighter->GetActorLocation();

		for (int32 i = 0; i < BurstCount; ++i)
		{
			AAbility* SpawnedAbility = Cast<AAbility>(Fighter->GetWorld()->SpawnActor(AbilityClass.Get(), &SpawnLocation));
			ensure(SpawnedAbility != nullptr);
			SpawnedAbility->SetCaster(Fighter);

			AProjectileAbility* ProjectileAbility = Cast<AProjectileAbility>(SpawnedAbility);
			ensure(ProjectileAbility != nullptr);
			if (ProjectileAbility != nullptr)
			{
				//float SpreadAngle = FMath::RandRange(0.0f, SpreadAngle);
				//float RadialAngle = FMath::RandRange(0.0f, 360.0f);
				FVector Direction = Fighter->GetActorForwardVector();

				ProjectileAbility->InitDirection(Direction);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan, TEXT("ProjectileGem does not have a ProjectileAbility assigned!"));
			}
		}
	}
}

void UProjectileGem::Release(AFighter* Fighter)
{
	Super::Release(Fighter);
}

void UProjectileGem::Tick(AFighter* Fighter, float DeltaTime)
{
	Super::Tick(Fighter, DeltaTime);
}
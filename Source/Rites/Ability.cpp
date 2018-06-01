// Fill out your copyright notice in the Description page of Project Settings.

#include "Ability.h"
#include "UnrealNetwork.h"
#include "Fighter.h"

// Sets default values
AAbility::AAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Caster = nullptr;
	bTimed = true;
	Lifetime = 1.0f;
	bInvulnerable = true;
	Health = 100;

	bReplicates = true;
	bReplicateMovement = false;
}

// Called when the game starts or when spawned
void AAbility::BeginPlay()
{
	Super::BeginPlay();
	TimeRemaining = Lifetime;

	OnDestroyed.AddDynamic(this, &AAbility::OnAbilityDestroyed);
}

void AAbility::OnAbilityDestroyed(AActor* Actor)
{
	ensure(Caster != nullptr);
	if (Caster != nullptr)
	{
		Caster->RemoveActiveAbility(this);
	}
}

// Called every frame
void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTimed)
	{
		TimeRemaining -= DeltaTime;

		if (TimeRemaining <= 0.0f &&
			HasAuthority())
		{
			Destroy();
		}
	}
}

// Accessors
AFighter* AAbility::GetCaster() const
{
	return Caster;
}

bool AAbility::IsTimed() const
{
	return bTimed;
}

float AAbility::GetLifetime() const
{
	return Lifetime;
}

bool AAbility::IsInvulnerable() const
{
	return bInvulnerable;
}

int32 AAbility::GetHealth() const
{
	return Health;
}

float AAbility::GetTimeRemaining() const
{
	return TimeRemaining;
}

// Mutators
void AAbility::SetCaster(AFighter* NewCaster)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		if (Caster != nullptr)
		{
			Caster->RemoveActiveAbility(this);
		}

		Caster = NewCaster;
		
		if (Caster != nullptr)
		{
			Caster->AddActiveAbility(this);
		}
	}
}

void AAbility::SetInvulnerable(bool bNewInvulnerable)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		bInvulnerable = bNewInvulnerable;
	}
}

void AAbility::SetHealth(int32 NewHealth)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		Health = NewHealth;
	}
}

void AAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAbility, Caster);
	DOREPLIFETIME(AAbility, bInvulnerable);
	DOREPLIFETIME(AAbility, Health);
}

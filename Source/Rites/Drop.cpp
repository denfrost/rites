// Fill out your copyright notice in the Description page of Project Settings.

#include "Drop.h"
#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Engine/World.h"

// Sets default values
ADrop::ADrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	MeshComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RootComponent = MeshComponent;
	ParticleComponent->SetupAttachment(MeshComponent);
	ParticleComponent->SecondsBeforeInactive = 0.0f;

	bReplicates = true;
}

UItem* ADrop::CreateItem() const
{
	return NewObject<UItem>(GetTransientPackage(), ItemClass);
}

// Called when the game starts or when spawned
void ADrop::BeginPlay()
{
	Super::BeginPlay();

	ensure(ItemClass.Get() != nullptr);

	// Start particle system
	ParticleComponent->Activate();

	const float DropCastDistance = 50000.0f;
	
	// Attempt to move drop down until it hits a world static object.
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - DropCastDistance * FVector::UpVector, ECC_GameTraceChannel1, ECC_WorldStatic);

	if (HitResult.bBlockingHit)
	{
		SetActorLocation(HitResult.Location);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Drop.h"
#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADrop::ADrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	MeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	RootComponent = MeshComponent;
	ParticleComponent->SetupAttachment(MeshComponent);
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
	
}

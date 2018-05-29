// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class RITES_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditAnywhere)
	bool bTimed = true;

	UPROPERTY(EditAnywhere)
	float Duration = 1.0f;

	UPROPERTY(EditAnywhere)
	bool bInvulnerable = true;

	UPROPERTY(EditAnywhere, Replicated)
	int32 Health = 100;

	UPROPERTY(Replicated)
	float TimeRemaining;
};

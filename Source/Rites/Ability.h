// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;
class AFighter;

UCLASS()
class RITES_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EnableOverlaps(bool bEnable);

	// Accessors
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AFighter* GetCaster() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTimed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetLifetime() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInvulnerable() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTimeRemaining() const;

	// Mutators
	UFUNCTION(BlueprintCallable)
	virtual void SetCaster(AFighter* NewCaster);

	UFUNCTION(BlueprintCallable)
	void SetInvulnerable(bool bNewInvulnerable);

	UFUNCTION(BlueprintCallable)
	void SetHealth(int32 NewHealth);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnAbilityDestroyed(AActor* Actor);

	UPROPERTY(VisibleAnywhere, Replicated)
	AFighter* Caster;

	UPROPERTY(EditAnywhere)
	bool bTimed;

	UPROPERTY(EditAnywhere)
	float Lifetime;

	UPROPERTY(EditAnywhere, Replicated)
	bool bInvulnerable;

	UPROPERTY(EditAnywhere, Replicated)
	int32 Health;

	UPROPERTY(VisibleAnywhere)
	float TimeRemaining;
};

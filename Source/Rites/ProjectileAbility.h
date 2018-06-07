// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "ProjectileAbility.generated.h"

class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class RITES_API AProjectileAbility : public AAbility
{
	GENERATED_BODY()
	
public:

	AProjectileAbility();

	virtual void Tick(float DeltaTime) override;

	virtual void EnableOverlaps(bool bEnable) override;

	UFUNCTION()
	void BeginOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void InitDirection(FVector Direction);

	UFUNCTION(BlueprintCallable)
	virtual void SetCaster(AFighter* NewCaster) override;

	// Accessors
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetGravity() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMinimumDampenedDamage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetDampenDamageOverLifetime() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetBounceCount() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPiercing() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetVelocity() const;

	// Mutators

	UFUNCTION(BlueprintCallable)
	void SetGravity(float NewGravity);

	UFUNCTION(BlueprintCallable)
	void SetDamage(float NewDamage);

	UFUNCTION(BlueprintCallable)
	void SetBounceCount(int32 NewBounceCount);

	UFUNCTION(BlueprintCallable)
	void SetPiercing(bool bNewPiercing);

	// TODO: Velocity is not replicated, so changing the velocity will require
	// a remote method invocation to update the velocity on the client.
	//UFUNCTION(BlueprintCallable)
	//void SetVelocity(FVector NewVelocity);

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_InitialVelocity();

	UFUNCTION()
	void OnProjectileDestroyed(AActor* Actor);

	// It should be assumed that all projectiles use a mesh.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere, Replicated)
	float Gravity;

	UPROPERTY(EditAnywhere, Replicated)
	float Damage;

	UPROPERTY(EditAnywhere)
	float MinimumDampenedDamage;

	UPROPERTY(EditAnywhere)
	bool bDampenDamageOverLifetime;

	UPROPERTY(EditAnywhere, Replicated)
	int32 BounceCount;

	UPROPERTY(EditAnywhere, Replicated)
	bool bPiercing;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_InitialVelocity)
	FVector InitialVelocity;

	UPROPERTY(VisibleAnywhere)
	FVector Velocity;

	UPROPERTY(EditAnywhere)
	bool bHitFriendly;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticle;
};

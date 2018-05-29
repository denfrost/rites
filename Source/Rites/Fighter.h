// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "FighterStats.h"
#include "InputState.h"

#include "Fighter.generated.h"

class UFighterAnimInstance;
class UFighterMovementComponent;
class ADrop;

UCLASS()
class RITES_API AFighter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFighter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Move(FVector Direction);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void BeginPickupSphereOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndBeginPickupSphereOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateJump(float DeltaTime, const FInputState& InputState);

	void UpdateMovementXY(float DeltaTime, const FInputState& InputState);

	void UpdateMovementZ(float DeltaTime, const FInputState& InputState);

	void UpdateOrientation(float DeltaTime, const FInputState& InputState);

	void UpdateCasting(float DeltaTime, const FInputState& InputState);

	void UpdateRecharge(float DeltaTime, const FInputState& InputState);

	void UpdateActivate(float DeltaTime, const FInputState& InputState);

	void UpdateAnimationState();

// Network Functions

	UFUNCTION(Server, Reliable, WithValidation)
	void S_SyncTransform(FVector location, FRotator rotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_SyncAnimState(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded);

	UFUNCTION(NetMulticast, Unreliable)
	void M_SyncAnimState(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded);

// Components

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* PickupSphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HeadMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ChestMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* LegMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* LeftHandMeshComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* RightHandMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UFighterMovementComponent* MovementComponent;

// Properties

	UPROPERTY(EditAnywhere, Replicated)
	FFighterStats Stats;

	UFighterAnimInstance* AnimInstance;

	FInputState PreviousInputState;

	bool bGrounded;
	bool bJumping;

	FVector2D InputDirection;
	FVector MovementVelocity;

	FVector ExternalVelocity;

	TArray<ADrop*> DropsInPickupRadius;
};

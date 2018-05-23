// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "FighterStats.h"
#include "InputState.h"

#include "Fighter.generated.h"

class UFighterAnimInstance;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateMovement(float DeltaTime, const FInputState& InputState);

	void UpdateOrientation(float DeltaTime, const FInputState& InputState);

	void UpdateCasting(float DeltaTime, const FInputState& InputState);

	void UpdateRecharge(float DeltaTime, const FInputState& InputState);

	void UpdateActivate(float DeltaTime, const FInputState& InputState);

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;
	
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

	UPROPERTY(EditAnywhere)
	FFighterStats Stats;

	UFighterAnimInstance* AnimInstance;

	FInputState PreviousInputState;
};

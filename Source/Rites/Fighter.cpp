// Fill out your copyright notice in the Description page of Project Settings.

#include "Fighter.h"
#include "FighterController.h"
#include "FighterAnimInstance.h"
#include "FighterMovementComponent.h"
#include "Utilities.h"
#include "Constants.h"
#include "Engine/Engine.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	HeadMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	ChestMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMesh"));
	LegMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	LeftHandMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	RightHandMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	MovementComponent = CreateDefaultSubobject<UFighterMovementComponent>(TEXT("Movement"));

	RootComponent = CapsuleComponent;
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
	BodyMeshComponent->SetupAttachment(RootComponent);
	HeadMeshComponent->SetupAttachment(BodyMeshComponent);
	ChestMeshComponent->SetupAttachment(BodyMeshComponent);
	LegMeshComponent->SetupAttachment(BodyMeshComponent);
	LeftHandMeshComponent->SetupAttachment(BodyMeshComponent);
	RightHandMeshComponent->SetupAttachment(BodyMeshComponent);

	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	MovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<UFighterAnimInstance>(BodyMeshComponent->GetAnimInstance());
	ensure(AnimInstance != nullptr);

	// Set this anim instance to control the other skeletal meshes
	HeadMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	ChestMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	LegMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	LeftHandMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	RightHandMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AFighterController* FighterController = Cast<AFighterController>(GetController());

	if (FighterController != nullptr)
	{
		const FInputState& InputState = FighterController->GetInputState();

		UpdateJump(DeltaTime, InputState);
		UpdateMovementXY(DeltaTime, InputState);
		UpdateMovementZ(DeltaTime, InputState);
		UpdateOrientation(DeltaTime, InputState);
		UpdateCasting(DeltaTime, InputState);
		UpdateRecharge(DeltaTime, InputState);
		UpdateActivate(DeltaTime, InputState);
		UpdateAnimationState(DeltaTime, InputState);

		PreviousInputState = InputState;
	}
}

void AFighter::Move(FVector Direction)
{
	Direction;
}

void AFighter::UpdateJump(float DeltaTime, const FInputState& InputState)
{
	if (bGrounded &&
		InputState.bJumpDown &&
		!PreviousInputState.bJumpDown)
	{
		bGrounded = false;
		bJumping = true;
		ExternalVelocity.Z = Stats.JumpSpeed;
	}
}

void AFighter::UpdateMovementXY(float DeltaTime, const FInputState& InputState)
{
	FVector OldLocation = GetActorLocation();

	FVector MoveDirection = FVector(-InputState.MoveDirection.X, InputState.MoveDirection.Y, 0.0f).RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);
	MoveDirection.Normalize();

	MovementVelocity = MoveDirection * Stats.MoveSpeed;

	if (InputState.MoveDirection.Y < 0.0f)
	{
		MovementVelocity *= Constants::FighterBackupSpeedMultiplier;
	}

	MovementComponent->Move(DeltaTime, MovementVelocity);
}

void AFighter::UpdateMovementZ(float DeltaTime, const FInputState& InputState)
{
	// If not grounded, then apply gravity and then check to see if the fighter is now grounded.
	if (!bGrounded)
	{
		if (bJumping &&
			InputState.bJumpDown)
		{
			ExternalVelocity.Z -= Constants::Gravity * DeltaTime * Constants::JumpGravityRatio;
		}
		else
		{
			ExternalVelocity.Z -= Constants::Gravity * DeltaTime;
		}

		bool bUpwardCollision = false;
		bGrounded = MovementComponent->MoveVert(DeltaTime, ExternalVelocity.Z, bUpwardCollision);

		if (bUpwardCollision)
		{
			ExternalVelocity.Z = 0.0f;
		}

		if (bGrounded)
		{
			ExternalVelocity.Z = 0.0f;
			bJumping = false;
		}
	}

	// If the fighter is grounded, then attempt to snap him to the ground
	// If there is no ground below, then set the fighter as not grounded.
	if (bGrounded)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = CurrentLocation;
		NewLocation.Z -= Constants::GroundedSnapDistance;

		FHitResult HitResult;
		SetActorLocation(NewLocation, true, &HitResult);

		if (HitResult.Actor != nullptr)
		{
			// The hero is hitting some collision mesh but...
			// We need to make sure the angle of impact is not too steep.
			// If it is too steep, then the hero is no longer grounded and
			// will be affected by gravity.
			float Incline = Utils::FindAngleBetweenVectors(HitResult.ImpactNormal, FVector::UpVector);

			if (Incline > Constants::MaxGroundingInclineAngle)
			{
				bGrounded = false;
			}
		}
		else
		{
			SetActorLocation(CurrentLocation, false);
			bGrounded = false;
		}
	}
}

void AFighter::UpdateOrientation(float DeltaTime, const FInputState& InputState)
{
	// TODO: Replace the constant sensitivity with user setting sensitivity.
	const float TempSensitivity = 40.0f;
	const float MinSpringArmPitch = -80.0f;
	const float MaxSpringArmPitch = 80.0f;

	FRotator NewActorRotation = GetActorRotation();
	FRotator NewSpringArmRotation = SpringArmComponent->RelativeRotation;

	float DeltaYaw = InputState.RotateDirection.X * TempSensitivity * DeltaTime;
	float DeltaPitch = InputState.RotateDirection.Y * TempSensitivity * DeltaTime;

	NewActorRotation.Yaw = FMath::Fmod(NewActorRotation.Yaw + DeltaYaw, 360.0f);
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch + DeltaPitch, MinSpringArmPitch, MaxSpringArmPitch);

	SetActorRotation(NewActorRotation);
	SpringArmComponent->SetRelativeRotation(NewSpringArmRotation);
}

void AFighter::UpdateCasting(float DeltaTime, const FInputState& InputState)
{

}

void AFighter::UpdateRecharge(float DeltaTime, const FInputState& InputState)
{

}

void AFighter::UpdateActivate(float DeltaTime, const FInputState& InputState)
{

}

void AFighter::UpdateAnimationState(float DeltaTime, const FInputState& InputState)
{
	AnimInstance->bJumping = bJumping;
	AnimInstance->bGrounded = bGrounded;
	AnimInstance->Velocity = MovementVelocity;
	AnimInstance->InputDirection = InputState.MoveDirection;
}
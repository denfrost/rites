// Fill out your copyright notice in the Description page of Project Settings.

#include "Fighter.h"
#include "FighterController.h"
#include "FighterAnimInstance.h"
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

	RootComponent = CapsuleComponent;
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
	BodyMeshComponent->SetupAttachment(RootComponent);
	HeadMeshComponent->SetupAttachment(BodyMeshComponent);
	ChestMeshComponent->SetupAttachment(BodyMeshComponent);
	LegMeshComponent->SetupAttachment(BodyMeshComponent);
	LeftHandMeshComponent->SetupAttachment(BodyMeshComponent);
	RightHandMeshComponent->SetupAttachment(BodyMeshComponent);
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

		UpdateMovement(DeltaTime, InputState);
		UpdateOrientation(DeltaTime, InputState);
		UpdateCasting(DeltaTime, InputState);
		UpdateRecharge(DeltaTime, InputState);
		UpdateActivate(DeltaTime, InputState);
	}
}

void AFighter::Move(FVector Direction)
{
	Direction;
}

void AFighter::UpdateMovement(float DeltaTime, const FInputState& InputState)
{
	FVector OldLocation = GetActorLocation();
	FVector MoveDirection = FVector(InputState.MoveDirection.X, InputState.MoveDirection.Y, 0.0f);
	FVector MoveVelocity = MoveDirection * Stats.MoveSpeed;
	FVector NewLocation = OldLocation + (MoveVelocity * DeltaTime);

	SetActorLocation(NewLocation);
	
	// Update Animation Variables
	AnimInstance->Velocity = MoveVelocity;
	AnimInstance->InputDirection = InputState.MoveDirection;
}

void AFighter::UpdateOrientation(float DeltaTime, const FInputState& InputState)
{
	// TODO: Replace the constant sensitivity with user setting sensitivity.
	const float TempSensitivity = 10.0f;
	const float MinSpringArmPitch = -50.0f;
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
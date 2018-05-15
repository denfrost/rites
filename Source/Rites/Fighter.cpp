// Fill out your copyright notice in the Description page of Project Settings.

#include "Fighter.h"


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
	
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFighter::Move(FVector Direction)
{
	Direction;
}

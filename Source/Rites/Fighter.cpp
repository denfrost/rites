// Fill out your copyright notice in the Description page of Project Settings.

#include "Fighter.h"
#include "FighterController.h"
#include "FighterAnimInstance.h"
#include "FighterMovementComponent.h"
#include "Utilities.h"
#include "Constants.h"
#include "Drop.h"
#include "Item.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	PickupSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
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
	PickupSphereComponent->SetupAttachment(RootComponent);
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

	PickupSphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	PickupSphereComponent->bGenerateOverlapEvents = true;
	PickupSphereComponent->InitSphereRadius(120.0f);

	PickupSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFighter::BeginPickupSphereOverlap);
	PickupSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AFighter::EndBeginPickupSphereOverlap);

	bReplicates = true;
	bReplicateMovement = true;
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

void AFighter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFighter, Stats);
}


void AFighter::BeginPickupSphereOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (HasAuthority() || IsLocallyControlled())
	{
		ADrop* OtherDrop = Cast<ADrop>(OtherActor);

		if (OtherDrop != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Drop Add To Pickup List"));
			DropsInPickupRadius.Add(OtherDrop);
		}
	}
}

void AFighter::EndBeginPickupSphereOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority() || IsLocallyControlled())
	{
		ADrop* OtherDrop = Cast<ADrop>(OtherActor);

		if (OtherDrop != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Drop Removed From Pickup List"));
			DropsInPickupRadius.Remove(OtherDrop);
		}
	}
}

TArray<UItem*> AFighter::GetCarriedItems()
{
	return CarriedItems;
}

TArray<ADrop*> AFighter::GetDropsInPickupRadius()
{
	return DropsInPickupRadius;
}

void AFighter::PickupItem(int32 ItemInstanceID)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("Pickup!"));
	S_PickupItem(ItemInstanceID);
}

void AFighter::DropItem(int32 ItemInstanceID)
{
	S_DropItem(ItemInstanceID);
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AFighterController* FighterController = Cast<AFighterController>(GetController());

	if (IsLocallyControlled() &&
		FighterController != nullptr)
	{
		const FInputState& InputState = FighterController->GetInputState();

		UpdateJump(DeltaTime, InputState);
		UpdateMovementXY(DeltaTime, InputState);
		UpdateMovementZ(DeltaTime, InputState);
		UpdateOrientation(DeltaTime, InputState);
		UpdateCasting(DeltaTime, InputState);
		UpdateRecharge(DeltaTime, InputState);
		UpdateActivate(DeltaTime, InputState);

		S_SyncTransform(GetActorLocation(), GetActorRotation());
		S_SyncAnimState(InputDirection, MovementVelocity, bJumping, bGrounded);

		PreviousInputState = InputState;
	}

	UpdateAnimationState();
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
	InputDirection = InputState.MoveDirection;

	FVector MoveDirection = FVector(-InputDirection.X, InputDirection.Y, 0.0f).RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);
	MoveDirection.Normalize();

	MovementVelocity = MoveDirection * Stats.MoveSpeed;

	// Apply backwards movement penalty
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

void AFighter::UpdateAnimationState()
{
	AnimInstance->bJumping = bJumping;
	AnimInstance->bGrounded = bGrounded;
	AnimInstance->Velocity = MovementVelocity;
	AnimInstance->InputDirection = InputDirection;
}

void AFighter::S_SyncTransform_Implementation(FVector location, FRotator rotation)
{
	SetActorLocation(location);
	SetActorRotation(rotation);
}

bool AFighter::S_SyncTransform_Validate(FVector location, FRotator rotation)
{
	return true;
}

void AFighter::S_SyncAnimState_Implementation(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded)
{
	this->InputDirection = InputDirection;
	this->MovementVelocity = Velocity;
	this->bJumping = bJumping;
	this->bGrounded = bGrounded;

	M_SyncAnimState(InputDirection, Velocity, bJumping, bGrounded);
}

bool AFighter::S_SyncAnimState_Validate(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded)
{
	return true;
}

void AFighter::M_SyncAnimState_Implementation(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded)
{
	if (!IsLocallyControlled())
	{
		this->bJumping = bJumping;
		this->bGrounded = bGrounded;
		this->MovementVelocity = Velocity;
		this->InputDirection = InputDirection;
	}
}

void AFighter::S_PickupItem_Implementation(int32 ItemInstanceID)
{
	ensure(HasAuthority());

	if (CarriedItems.Num() < Constants::FighterMaxCarriedItems)
	{
		for (int32 i = 0; i < DropsInPickupRadius.Num(); ++i)
		{
			ensure(DropsInPickupRadius[i] != nullptr);

			if (DropsInPickupRadius[i] != nullptr &&
				DropsInPickupRadius[i]->GetItem() != nullptr &&
				DropsInPickupRadius[i]->GetItem()->GetInstanceID() == ItemInstanceID)
			{
				UItem* PickedUpItem = DropsInPickupRadius[i]->GetItem();
				CarriedItems.Add(PickedUpItem);
				DropsInPickupRadius[i]->Destroy();

				if (!IsLocallyControlled())
				{
					C_AddItem(PickedUpItem->GetItemData());
				}
				
				break;
			}
		}
	}
}

bool AFighter::S_PickupItem_Validate(int32 ItemInstanceID)
{
	return true;
}

void AFighter::C_AddItem_Implementation(FItemData ItemData)
{
	ensure(IsLocallyControlled());
	UItem* NewItem = NewObject<UItem>(GetTransientPackage(), ItemData.ItemClass);
	NewItem->SetItemData(ItemData);
	CarriedItems.Add(NewItem);
}

void AFighter::S_DropItem_Implementation(int32 ItemInstanceID)
{
	ensure(HasAuthority());

	for (int32 i = 0; i < CarriedItems.Num(); ++i)
	{
		ensure(CarriedItems[i] != nullptr);

		if (CarriedItems[i] != nullptr &&
			CarriedItems[i]->GetInstanceID() == ItemInstanceID)
		{	
			FVector SpawnLocation = GetActorLocation();
			ADrop* SpawnedDrop = Cast<ADrop>(GetWorld()->SpawnActor(CarriedItems[i]->GetDropClass().Get(), &SpawnLocation));

			SpawnedDrop->SetItemData(CarriedItems[i]->GetItemData());
			CarriedItems.RemoveAt(i);

			if (!IsLocallyControlled())
			{
				C_RemoveItem(ItemInstanceID);
			}
		}
	}
}

bool AFighter::S_DropItem_Validate(int32 ItemInstanceID)
{
	return true;
}

void AFighter::C_RemoveItem_Implementation(int32 ItemInstanceID)
{
	ensure(IsLocallyControlled());

	for (int32 i = 0; i < CarriedItems.Num(); ++i)
	{
		ensure(CarriedItems[i] != nullptr);
		if (CarriedItems[i] != nullptr &&
			CarriedItems[i]->GetInstanceID() == ItemInstanceID)
		{
			CarriedItems.RemoveAt(i);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Fighter.h"
#include "FighterController.h"
#include "FighterAnimInstance.h"
#include "FighterMovementComponent.h"
#include "Utilities.h"
#include "Constants.h"
#include "Drop.h"
#include "Item.h"
#include "Gear.h"
#include "Gem.h"
#include "ArenaGameMode.h"
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
	ProjectileSpawnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
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
	ProjectileSpawnComponent->SetupAttachment(BodyMeshComponent, TEXT("Neck"));

	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	MovementComponent->UpdatedComponent = RootComponent;

	BodyMeshComponent->bGenerateOverlapEvents = true;
	BodyMeshComponent->SetCollisionProfileName(TEXT("PawnHit"));

	PickupSphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	PickupSphereComponent->bGenerateOverlapEvents = true;
	PickupSphereComponent->InitSphereRadius(120.0f);

	PickupSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFighter::BeginPickupSphereOverlap);
	PickupSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AFighter::EndBeginPickupSphereOverlap);

	EquippedGear.AddDefaulted(static_cast<int32>(EGearSlot::Count));

	bReplicates = true;
	bReplicateMovement = true;
	NetUpdateFrequency = 60.0f;
}

// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<UFighterAnimInstance>(BodyMeshComponent->GetAnimInstance());
	ensure(AnimInstance != nullptr);

	AnimInstance->Fighter = this;

	// Set this anim instance to control the other skeletal meshes
	HeadMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	ChestMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	LegMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	LeftHandMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
	RightHandMeshComponent->SetMasterPoseComponent(BodyMeshComponent);

	// Save off the initial stats so they can be used to replace stats when respawning the fighter
	InitialStats = Stats;
}

void AFighter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFighter, Stats);
	DOREPLIFETIME(AFighter, ActiveAbilities);
	DOREPLIFETIME(AFighter, LookAngle);
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
			DirtyInventoryMenu();
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
			DirtyInventoryMenu();
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

TArray<UGear*> AFighter::GetEquippedGear()
{
	return EquippedGear;
}

EGearSlot AFighter::ConvertIntToGearSlot(int32 Index)
{
	EGearSlot ReturnSlot = EGearSlot::Count;

	if (Index >= 0 && Index < static_cast<int32>(EGearSlot::Count))
	{
		ReturnSlot = static_cast<EGearSlot>(Index);
	}

	return ReturnSlot;
}

float AFighter::GetGlobalCooldownRemainin() const
{
	return GlobalCooldownRemaining;
}

bool AFighter::IsOnGlobalCooldown() const
{
	return GlobalCooldownRemaining > 0.0f;
}

float AFighter::GetLookAngle() const
{
	return LookAngle;
}

bool AFighter::IsFriendly(AFighter* OtherFighter)
{
	// TODO: Implement team logic
	return OtherFighter == this;
}

USkeletalMeshComponent* AFighter::GetBodyMeshComponent()
{
	return BodyMeshComponent;
}

FFighterStats AFighter::GetFighterStats() const
{
	return Stats;
}

void AFighter::SetCastLeftAnimState(bool bCastLeft)
{
	this->bCastLeft = bCastLeft;
}

void AFighter::SetCastRightAnimState(bool bCastRight)
{
	this->bCastRight = bCastRight;
}

void AFighter::Damage(int32 Damage)
{
	S_Damage(Damage);
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

void AFighter::EquipItem(int32 ItemInstanceID)
{
	S_EquipItem(ItemInstanceID);
}

void AFighter::UnequipItem(int32 ItemInstanceID, EGearSlot GearSlot)
{
	S_UnequipItem(ItemInstanceID, GearSlot);
}

void AFighter::SocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	S_SocketGem(GearInstanceID, GearSlot, GemInstanceID);
}

void AFighter::UnsocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	S_UnsocketGem(GearInstanceID, GearSlot, GemInstanceID);
}

UFUNCTION(BlueprintCallable)
void AFighter::DropAllItems()
{
	// Drop inventory
	for (int32 i = CarriedItems.Num() - 1; i >= 0 ; --i)
	{
		ensure(CarriedItems[i] != nullptr);

		if (CarriedItems[i] != nullptr)
		{
			S_DropItem(CarriedItems[i]->GetInstanceID());
		}
	}

	// Drop all equipment
	for (int32 i = 0; i < EquippedGear.Num(); ++i)
	{
		if (EquippedGear[i] != nullptr)
		{
			int32 InstanceID = EquippedGear[i]->GetInstanceID();
			S_UnequipItem(InstanceID, static_cast<EGearSlot>(i));
			S_DropItem(InstanceID);
		}
	}
}

UFUNCTION(BlueprintCallable)
void AFighter::ResetStats()
{
	ensure(HasAuthority());

	if (HasAuthority())
	{
		Stats = InitialStats;
	}
}

UFUNCTION(BlueprintCallable)
void AFighter::Transport(FVector NewLocation)
{
	ensure(HasAuthority());

	if (HasAuthority())
	{
		SetActorLocation(NewLocation);
		C_Transport(NewLocation);
	}
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
		UpdateInteract(DeltaTime, InputState);
		UpdateGlobalCooldown(DeltaTime);
		UpdateGems(DeltaTime);

		S_SyncTransform(GetActorLocation(), GetActorRotation(), LookAngle);
		S_SyncAnimState(InputDirection, MovementVelocity, bJumping, bGrounded, bCastLeft, bCastRight);

		PreviousInputState = InputState;
	}
	else if (HasAuthority())
	{
		UpdateGlobalCooldown(DeltaTime);
		UpdateGems(DeltaTime);
	}

	UpdateAnimationState();

	// Equipped Gear array should never change size.
	ensure(EquippedGear.Num() == static_cast<int32>(EGearSlot::Count));
}

void AFighter::Move(FVector Direction)
{
	Direction;
}

void AFighter::SetGlobalCooldown(float CooldownTime)
{
	GlobalCooldownRemaining = CooldownTime;
}

void AFighter::SetLastActivatedGem(UGem* Gem)
{
	LastActivatedGem = Gem;
}

void AFighter::AddActiveAbility(AAbility* Ability)
{
	ActiveAbilities.Add(Ability);
}

void AFighter::RemoveActiveAbility(AAbility* Ability)
{
	ActiveAbilities.Remove(Ability);
}

FVector AFighter::GetProjectileSpawnLocation() const
{
	return ProjectileSpawnComponent->GetComponentLocation();
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

	FVector MoveDirection = FVector(InputDirection.Y, InputDirection.X, 0.0f).RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);
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

	LookAngle = SpringArmComponent->RelativeRotation.Pitch;
}

void AFighter::UpdateCasting(float DeltaTime, const FInputState& InputState)
{
	UpdateGemActivation(EGearSlot::LeftGlove, 0, InputState.bCastLeftDown, PreviousInputState.bCastLeftDown, DeltaTime);
	UpdateGemActivation(EGearSlot::LeftGlove, 1, InputState.bCastLeftSecondaryDown, PreviousInputState.bCastLeftSecondaryDown, DeltaTime);
	UpdateGemActivation(EGearSlot::RightGlove, 0, InputState.bCastRightDown, PreviousInputState.bCastRightDown, DeltaTime);
	UpdateGemActivation(EGearSlot::RightGlove, 1, InputState.bCastRightSecondaryDown, PreviousInputState.bCastRightSecondaryDown, DeltaTime);
}

void AFighter::UpdateRecharge(float DeltaTime, const FInputState& InputState)
{

}

void AFighter::UpdateInteract(float DeltaTime, const FInputState& InputState)
{

}


void AFighter::UpdateGlobalCooldown(float DeltaTime)
{
	GlobalCooldownRemaining -= DeltaTime;
}

void AFighter::UpdateGems(float DeltaTime)
{
	for (int32 i = 0; i < EquippedGear.Num(); ++i)
	{
		if (EquippedGear[i] != nullptr)
		{
			TArray<FGemSocket>& Sockets = EquippedGear[i]->GetSockets();

			for (int32 s = 0; s < Sockets.Num(); ++s)
			{
				if (Sockets[s].Gem != nullptr)
				{
					Sockets[s].Gem->Tick(this, DeltaTime);
				}
			}
		}
	}
}

void AFighter::UpdateAnimationState()
{
	AnimInstance->bJumping = bJumping;
	AnimInstance->bGrounded = bGrounded;
	AnimInstance->Velocity = MovementVelocity;
	AnimInstance->InputDirection = InputDirection;
	AnimInstance->LookAngle = LookAngle;
	AnimInstance->bCastLeft = bCastLeft;
	AnimInstance->bCastRight = bCastRight;
}

void AFighter::UpdateGemActivation(EGearSlot GearSlot, int32 SocketIndex, bool ButtonDown, bool PreviousButtonDown, float DeltaTime)
{
	UGem* Gem = GetEquippedGem(GearSlot, SocketIndex);

	if (Gem != nullptr)
	{
		// We found a gem. Now call the appropriate function (Active / Release / Tick) based on pressed buttons.
		if (ButtonDown && !PreviousButtonDown &&
			(!IsOnGlobalCooldown() || LastActivatedGem == Gem) &&
			!Gem->IsOnCooldown() &&
			(Gem->GetMaxChargers() == 0 || Gem->GetCharges() > 0))
		{
			// Activate the gem if off cooldown.
			S_ActivateGem(GearSlot, SocketIndex);

			if (GearSlot == EGearSlot::LeftGlove)
			{
				bCastLeft = true;
			}
			else if (GearSlot == EGearSlot::RightGlove)
			{
				bCastRight = true;
			}
		}
		else if (!ButtonDown &&
				Gem->IsCurrentlyBeingChanneled())
		{
			// "Release()" is called on channeled gems once the user lets go of the casting button
			S_ReleaseGem(GearSlot, SocketIndex);
		}
	}
}

EGearSlot AFighter::GetAvailableSlotForGear(UGear* Gear)
{
	EGearSlot ReturnSlot = EGearSlot::Count;
	EGearSlot CandidateSlot = EGearSlot::Count;

	// Equipped Gear array should never change size.
	ensure(EquippedGear.Num() == static_cast<int32>(EGearSlot::Count));

	if (Gear != nullptr)
	{
		EGearType GearType = Gear->GetType();

		switch (GearType)
		{
		case EGearType::Head:
			CandidateSlot = EGearSlot::Head;
			break;
		case EGearType::Chest:
			CandidateSlot = EGearSlot::Chest;
			break;
		case EGearType::Legs:
			CandidateSlot = EGearSlot::Legs;
			break;
		case EGearType::Glove:
			if (EquippedGear[static_cast<int32>(EGearSlot::LeftGlove)] == nullptr)
				CandidateSlot = EGearSlot::LeftGlove;
			else
				CandidateSlot = EGearSlot::RightGlove;
			break;
		}

		if (CandidateSlot != EGearSlot::Count &&
			EquippedGear[static_cast<int32>(CandidateSlot)] == nullptr)
		{
			ReturnSlot = CandidateSlot;
		}
	}

	return ReturnSlot;
}

bool AFighter::MoveCarriedItemToEquipSlot(int32 ItemInstanceID)
{
	bool bSucessful = false;

	for (int32 i = 0; i < CarriedItems.Num(); ++i)
	{
		UGear* Gear = Cast<UGear>(CarriedItems[i]);
		ensure(CarriedItems[i] != nullptr);

		if (Gear != nullptr &&
			Gear->GetInstanceID() == ItemInstanceID)
		{
			EGearType GearType = Gear->GetType();

			EGearSlot Slot = GetAvailableSlotForGear(Gear);
			int32 SlotIndex = static_cast<int32>(Slot);

			if (SlotIndex < static_cast<int32>(EGearSlot::Count) &&
				EquippedGear[SlotIndex] == nullptr)
			{
				EquippedGear[SlotIndex] = Gear;
				CarriedItems.RemoveAt(i);
				bSucessful = true;
				break;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Orange, TEXT("Gear slot is already taken"));
			}
		}
	}

	return bSucessful;
}

bool AFighter::MoveEquippedItemToCarried(int32 ItemInstanceID, EGearSlot GearSlot)
{
	bool bSuccessful = false;

	if (GearSlot < EGearSlot::Count &&
		CarriedItems.Num() < Constants::FighterMaxCarriedItems)
	{
		UGear* Gear = EquippedGear[static_cast<int32>(GearSlot)];

		if (Gear != nullptr &&
			Gear->GetInstanceID() == ItemInstanceID)
		{
			CarriedItems.Add(EquippedGear[static_cast<int32>(GearSlot)]);
			EquippedGear[static_cast<int32>(GearSlot)] = nullptr;
			bSuccessful = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Could not find gear to unequip"));
		}
	}

	return bSuccessful;
}

bool AFighter::MoveCarriedGemToGearSocket(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	bool bSuccessful = false;

	UGem* Gem = nullptr;
	UGear* Gear = nullptr;
	int32 GemCarriedIndex = 0;

	// Find Gem first
	for (int32 i = 0; i < CarriedItems.Num(); ++i)
	{
		if (CarriedItems[i] != nullptr &&
			CarriedItems[i]->GetInstanceID() == GemInstanceID)
		{
			Gem = Cast<UGem>(CarriedItems[i]);
			GemCarriedIndex = i;
		}
	}

	if (GearSlot < EGearSlot::Count)
	{
		Gear = EquippedGear[static_cast<int32>(GearSlot)];
	}

	// Attach if gear is valid and compatible socket is open
	if (Gem != nullptr &&
		Gear != nullptr &&
		Gear->GetInstanceID() == GearInstanceID)
	{
		TArray<FGemSocket>& Sockets = Gear->GetSockets();

		for (int32 s = 0; s < Sockets.Num(); ++s)
		{
			if (Sockets[s].Gem == nullptr &&
				Sockets[s].Color == Gem->GetColor())
			{
				// Valid socket was found
				Sockets[s].Gem = Cast<UGem>(Gem);
				CarriedItems.RemoveAt(GemCarriedIndex);
				bSuccessful = true;
				break;
			}
		}
	}

	return bSuccessful;
}

bool AFighter::MoveSocketedGemToCarried(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	bool bSuccessful = false;

	if (GearSlot < EGearSlot::Count &&
		CarriedItems.Num() < Constants::FighterMaxCarriedItems)
	{
		UGear* Gear = EquippedGear[static_cast<int32>(GearSlot)];

		if (Gear != nullptr &&
			Gear->GetInstanceID() == GearInstanceID)
		{
			TArray<FGemSocket>& Sockets = Gear->GetSockets();

			for (int32 s = 0; s < Sockets.Num(); ++s)
			{
				if (Sockets[s].Gem != nullptr &&
					Sockets[s].Gem->GetInstanceID() == GemInstanceID)
				{
					// The gem was found, clear it on the gear and add it to carried array
					CarriedItems.Add(Sockets[s].Gem);
					Sockets[s].Gem = nullptr;
					bSuccessful = true;
					break;
				}
			}
		}
	}

	return bSuccessful;
}

UGem* AFighter::GetEquippedGem(EGearSlot GearSlot, int32 SocketIndex)
{
	UGem* Gem = nullptr;
	UGear* Gear = EquippedGear[static_cast<int32>(GearSlot)];

	if (Gear != nullptr)
	{	
		TArray<FGemSocket>& Sockets = Gear->GetSockets();

		if (SocketIndex < Sockets.Num())
		{
			Gem = Sockets[SocketIndex].Gem;
		}
	}

	return Gem;
}

void AFighter::S_SyncTransform_Implementation(FVector Location, FRotator Rotation, float NewLookAngle)
{
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	LookAngle = NewLookAngle;
}

bool AFighter::S_SyncTransform_Validate(FVector location, FRotator rotation, float LookAngle)
{
	return true;
}

void AFighter::S_SyncAnimState_Implementation(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded, bool bCastLeft, bool bCastRight)
{
	this->InputDirection = InputDirection;
	this->MovementVelocity = Velocity;
	this->bJumping = bJumping;
	this->bGrounded = bGrounded;
	this->bCastLeft = this->bCastLeft || bCastLeft;
	this->bCastRight = this->bCastRight || bCastRight;

	M_SyncAnimState(InputDirection, Velocity, bJumping, bGrounded, bCastLeft, bCastRight);
}

bool AFighter::S_SyncAnimState_Validate(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded, bool bCastLeft, bool bCastRight)
{
	return true;
}

void AFighter::M_SyncAnimState_Implementation(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded, bool bCastLeft, bool bCastRight)
{
	if (!IsLocallyControlled())
	{
		this->bJumping = bJumping;
		this->bGrounded = bGrounded;
		this->MovementVelocity = Velocity;
		this->InputDirection = InputDirection;
		this->bCastLeft = this->bCastLeft || bCastLeft;
		this->bCastRight = this->bCastRight || bCastRight;
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
					UGear* Gear = Cast<UGear>(PickedUpItem);

					if (Gear != nullptr)
					{
						if (Gear->GetSockets().Num() == 1)
						{
							FItemData Gem0Data = Gear->GetSockets()[0].Gem != nullptr ? Gear->GetSockets()[0].Gem->GetItemData() : FItemData();
							C_AddItem_1Socket(Gear->GetItemData(), Gem0Data);
						}
						else if (Gear->GetSockets().Num() == 2)
						{
							FItemData Gem0Data = Gear->GetSockets()[0].Gem != nullptr ? Gear->GetSockets()[0].Gem->GetItemData() : FItemData();
							FItemData Gem1Data = Gear->GetSockets()[1].Gem != nullptr ? Gear->GetSockets()[1].Gem->GetItemData() : FItemData();
							C_AddItem_2Socket(Gear->GetItemData(), Gem0Data, Gem1Data);
						}
						else
						{
							C_AddItem(Gear->GetItemData());
						}
					}
					else
					{
						C_AddItem(PickedUpItem->GetItemData());
					}
				}
				else
				{
					DirtyInventoryMenu();
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

	DirtyInventoryMenu();
}

void AFighter::C_AddItem_1Socket_Implementation(FItemData BaseItemData, FItemData SocketedGem0Data)
{
	ensure(IsLocallyControlled());

	UItem* NewItem = NewObject<UItem>(GetTransientPackage(), BaseItemData.ItemClass);
	NewItem->SetItemData(BaseItemData);

	UGear* Gear = Cast<UGear>(NewItem);

	ensure(Gear != nullptr);
	ensure(Gear->GetSockets().Num() >= 1);

	if (Gear != nullptr &&
		Gear->GetSockets().Num() >= 1)
	{
		Gear->GetSockets()[0].Gem = SocketedGem0Data.InstanceID != 0 ? NewObject<UGem>(GetTransientPackage(), SocketedGem0Data.ItemClass) : nullptr;
		
		if (Gear->GetSockets()[0].Gem != nullptr) Gear->GetSockets()[0].Gem->SetItemData(SocketedGem0Data);
	}

	CarriedItems.Add(NewItem);

	DirtyInventoryMenu();
}

void AFighter::C_AddItem_2Socket_Implementation(FItemData BaseItemData, FItemData SocketedGem0Data, FItemData SocketedGem1Data)
{
	ensure(IsLocallyControlled());

	UItem* NewItem = NewObject<UItem>(GetTransientPackage(), BaseItemData.ItemClass);
	NewItem->SetItemData(BaseItemData);

	UGear* Gear = Cast<UGear>(NewItem);

	ensure(Gear != nullptr);
	ensure(Gear->GetSockets().Num() >= 2);

	if (Gear != nullptr &&
		Gear->GetSockets().Num() >= 2)
	{
		Gear->GetSockets()[0].Gem = SocketedGem0Data.InstanceID != 0 ? NewObject<UGem>(GetTransientPackage(), SocketedGem0Data.ItemClass) : nullptr;
		Gear->GetSockets()[1].Gem = SocketedGem1Data.InstanceID != 0 ? NewObject<UGem>(GetTransientPackage(), SocketedGem1Data.ItemClass) : nullptr;

		if (Gear->GetSockets()[0].Gem != nullptr) Gear->GetSockets()[0].Gem->SetItemData(SocketedGem0Data);
		if (Gear->GetSockets()[1].Gem != nullptr) Gear->GetSockets()[1].Gem->SetItemData(SocketedGem1Data);
	}

	CarriedItems.Add(NewItem);

	DirtyInventoryMenu();
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

			SpawnedDrop->TransferItemToDrop(CarriedItems[i]);
			CarriedItems.RemoveAt(i);

			if (!IsLocallyControlled())
			{
				C_RemoveItem(ItemInstanceID);
			}
			else
			{
				DirtyInventoryMenu();
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

	DirtyInventoryMenu();
}

void AFighter::S_EquipItem_Implementation(int32 ItemInstanceID)
{
	ensure(HasAuthority());

	if (MoveCarriedItemToEquipSlot(ItemInstanceID))
	{
		if (!IsLocallyControlled())
		{
			C_EquipItem(ItemInstanceID);
		}
		else
		{
			DirtyInventoryMenu();
		}
	}
}

bool AFighter::S_EquipItem_Validate(int32 ItemInstanceID)
{
	return true;
}

void AFighter::C_EquipItem_Implementation(int32 ItemInstanceID)
{
	ensure(IsLocallyControlled());

	MoveCarriedItemToEquipSlot(ItemInstanceID);
	DirtyInventoryMenu();
}

void AFighter::S_UnequipItem_Implementation(int32 ItemInstanceID, EGearSlot GearSlot)
{
	ensure(HasAuthority());

	if (MoveEquippedItemToCarried(ItemInstanceID, GearSlot))
	{
		if (!IsLocallyControlled())
		{
			C_UnequipItem(ItemInstanceID, GearSlot);
		}
		else
		{
			DirtyInventoryMenu();
		}
	}
}

bool AFighter::S_UnequipItem_Validate(int32 ItemInstanceID, EGearSlot GearSlot)
{
	return true;
}

void AFighter::C_UnequipItem_Implementation(int32 ItemInstanceID, EGearSlot GearSlot)
{
	ensure(IsLocallyControlled());

	MoveEquippedItemToCarried(ItemInstanceID, GearSlot);
	DirtyInventoryMenu();
}

void AFighter::S_SocketGem_Implementation(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	ensure(HasAuthority());

	if (MoveCarriedGemToGearSocket(GearInstanceID, GearSlot, GemInstanceID))
	{
		if (!IsLocallyControlled())
		{
			C_SocketGem(GearInstanceID, GearSlot, GemInstanceID);
		}
		else
		{
			DirtyInventoryMenu();
		}
	}
}

bool AFighter::S_SocketGem_Validate(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	return true;
}

void AFighter::C_SocketGem_Implementation(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	ensure(IsLocallyControlled());

	MoveCarriedGemToGearSocket(GearInstanceID, GearSlot, GemInstanceID);
	DirtyInventoryMenu();
}

void AFighter::S_UnsocketGem_Implementation(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	ensure(HasAuthority());

	if (MoveSocketedGemToCarried(GearInstanceID, GearSlot, GemInstanceID))
	{
		if (!IsLocallyControlled())
		{
			C_UnsocketGem(GearInstanceID, GearSlot, GemInstanceID);
		}
		else
		{
			DirtyInventoryMenu();
		}
	}
}

bool AFighter::S_UnsocketGem_Validate(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	return true;
}

void AFighter::C_UnsocketGem_Implementation(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID)
{
	ensure(IsLocallyControlled());

	MoveSocketedGemToCarried(GearInstanceID, GearSlot, GemInstanceID);
	DirtyInventoryMenu();
}

void AFighter::S_ActivateGem_Implementation(EGearSlot GearSlot, int32 SocketIndex)
{
	ensure(HasAuthority());

	UGem* Gem = GetEquippedGem(GearSlot, SocketIndex);

	if (Gem != nullptr &&
		(!IsOnGlobalCooldown() || LastActivatedGem == Gem) &&
		!Gem->IsOnCooldown() &&
		(Gem->GetMaxChargers() == 0 || Gem->GetCharges() > 0))
	{
		Gem->Activate(this);

		if (!IsLocallyControlled())
		{
			C_ActivateGem(GearSlot, SocketIndex);
		}
	}
}

bool AFighter::S_ActivateGem_Validate(EGearSlot GearSlot, int32 SocketIndex)
{
	return true;
}

void AFighter::C_ActivateGem_Implementation(EGearSlot GearSlot, int32 SocketIndex)
{
	ensure(IsLocallyControlled());
	UGem* Gem = GetEquippedGem(GearSlot, SocketIndex);

	ensure(Gem != nullptr);
	if (Gem != nullptr)
	{
		Gem->RegisterActivate(this);
	}
}

void AFighter::S_ReleaseGem_Implementation(EGearSlot GearSlot, int32 SocketIndex)
{
	ensure(HasAuthority());
	UGem* Gem = GetEquippedGem(GearSlot, SocketIndex);

	if (Gem != nullptr &&
		Gem->IsCurrentlyBeingChanneled())
	{
		Gem->Release(this);

		if (!IsLocallyControlled())
		{
			C_ReleaseGem(GearSlot, SocketIndex);
		}
	}
}

bool AFighter::S_ReleaseGem_Validate(EGearSlot GearSlot, int32 SocketIndex)
{
	return true;
}

void AFighter::C_ReleaseGem_Implementation(EGearSlot GearSlot, int32 SocketIndex)
{
	ensure(IsLocallyControlled());

	UGem* Gem = GetEquippedGem(GearSlot, SocketIndex);

	ensure(Gem != nullptr);
	if (Gem != nullptr)
	{
		Gem->RegisterRelease(this);
	}
}

void AFighter::S_Damage_Implementation(float Damage)
{
	Stats.Health -= Damage;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Health: %d"), Stats.Health));
	
	DirtyHUDMenu(true);

	if (Stats.Health < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Dead"));
		AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GetWorld()->GetAuthGameMode());
		
		ensure(ArenaGameMode != nullptr);
		if (ArenaGameMode != nullptr)
		{
			ArenaGameMode->OnFighterKilled(this);
		}
	}
}

bool AFighter::S_Damage_Validate(float Damage)
{
	return true;
}

void AFighter::C_Transport_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

void AFighter::OnRep_Stats()
{
	DirtyHUDMenu(true);
}
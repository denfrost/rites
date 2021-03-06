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
#include "ItemData.h"
#include "Enums.h"

#include "Fighter.generated.h"

class UFighterAnimInstance;
class UFighterMovementComponent;
class ADrop;
class AAbility;
class UItem;
class UGear;
class UGem;
struct FItemData;

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

	void SetGlobalCooldown(float CooldownTime);

	void SetLastActivatedGem(UGem* Gem);

	void AddActiveAbility(AAbility* Ability);

	void RemoveActiveAbility(AAbility* Ability);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	FVector GetProjectileSpawnLocation() const;

	UFUNCTION()
	void BeginPickupSphereOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndBeginPickupSphereOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	TArray<UItem*> GetCarriedItems();

	UFUNCTION(BlueprintCallable)
	TArray<ADrop*> GetDropsInPickupRadius();

	UFUNCTION(BlueprintCallable)
	TArray<UGear*> GetEquippedGear();

	UFUNCTION(BlueprintCallable)
	EGearSlot ConvertIntToGearSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	float GetGlobalCooldownRemainin() const;

	UFUNCTION(BlueprintCallable)
	bool IsOnGlobalCooldown() const;

	UFUNCTION(BlueprintCallable)
	float GetLookAngle() const;

	UFUNCTION(BlueprintCallable)
	bool IsFriendly(AFighter* OtherFighter);

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetBodyMeshComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FFighterStats GetFighterStats() const;

	UFUNCTION(BlueprintCallable)
	void SetCastLeftAnimState(bool bCastLeft);

	UFUNCTION(BlueprintCallable)
	void SetCastRightAnimState(bool bCastRight);

	UFUNCTION(BlueprintCallable)
	void Damage(int32 Damage);

	UFUNCTION(BlueprintCallable)
	void PickupItem(int32 ItemInstanceID);

	UFUNCTION(BlueprintCallable)
	void DropItem(int32 ItemInstanceID);

	UFUNCTION(BlueprintCallable)
	void EquipItem(int32 ItemInstanceID);

	UFUNCTION(BlueprintCallable)
	void UnequipItem(int32 ItemInstanceID, EGearSlot GearSlot);

	UFUNCTION(BlueprintCallable)
	void SocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UFUNCTION(BlueprintCallable)
	void UnsocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UFUNCTION(BlueprintCallable)
	void DropAllItems();

	UFUNCTION(BlueprintCallable)
	void ResetStats();

	UFUNCTION(BlueprintCallable)
	void Transport(FVector NewLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateJump(float DeltaTime, const FInputState& InputState);

	void UpdateMovementXY(float DeltaTime, const FInputState& InputState);

	void UpdateMovementZ(float DeltaTime, const FInputState& InputState);

	void UpdateOrientation(float DeltaTime, const FInputState& InputState);

	void UpdateCasting(float DeltaTime, const FInputState& InputState);

	void UpdateRecharge(float DeltaTime, const FInputState& InputState);

	void UpdateInteract(float DeltaTime, const FInputState& InputState);

	void UpdateGlobalCooldown(float DeltaTime);

	void UpdateGems(float DeltaTime);

	void UpdateAnimationState();

	void UpdateGemActivation(EGearSlot GearSlot, int32 SocketIndex,bool ButtonDown, bool PreviousButtonDown, float DeltaTime);

	EGearSlot GetAvailableSlotForGear(UGear* Gear);

	bool MoveCarriedItemToEquipSlot(int32 ItemInstanceID);

	bool MoveEquippedItemToCarried(int32 ItemInstanceID, EGearSlot GearSlot);

	bool MoveCarriedGemToGearSocket(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	bool MoveSocketedGemToCarried(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UGem* GetEquippedGem(EGearSlot GearSlot, int32 SocketIndex);

// Blueprint Implementable Events

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DirtyInventoryMenu();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DirtyHUDMenu(bool bDirtyHealth);

// Network Functions

	UFUNCTION(Server, Reliable, WithValidation)
	void S_SyncTransform(FVector Location, FRotator Rotation, float LookAngle);

	UFUNCTION(Server, Unreliable, WithValidation)
	void S_SyncAnimState(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded, bool bCastLeft, bool bCastRight);

	UFUNCTION(NetMulticast, Unreliable)
	void M_SyncAnimState(FVector2D InputDirection, FVector Velocity, bool bJumping, bool bGrounded, bool bCastLeft, bool bCastRight);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_PickupItem(int32 ItemInstanceID);

	UFUNCTION(Client, Reliable)
	void C_AddItem(FItemData ItemData);

	UFUNCTION(Client, Reliable)
	void C_AddItem_1Socket(FItemData BaseItemData, FItemData SocketedGem0Data);
	
	UFUNCTION(Client, Reliable)
	void C_AddItem_2Socket(FItemData BaseItemData, FItemData SocketedGem0Data, FItemData SocketedGem1Data);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_DropItem(int32 ItemInstanceID);

	UFUNCTION(Client, Reliable)
	void C_RemoveItem(int32 ItemInstanceID);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_EquipItem(int32 ItemInstanceID);

	UFUNCTION(Client, Reliable)
	void C_EquipItem(int32 ItemInstanceID);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_UnequipItem(int32 ItemInstanceID, EGearSlot GearSlot);

	UFUNCTION(Client, Reliable)
	void C_UnequipItem(int32 ItemInstanceID, EGearSlot GearSlot);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_SocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UFUNCTION(Client, Reliable)
	void C_SocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_UnsocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UFUNCTION(Client, Reliable)
	void C_UnsocketGem(int32 GearInstanceID, EGearSlot GearSlot, int32 GemInstanceID);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_ActivateGem(EGearSlot GearSlot, int32 SocketIndex);

	UFUNCTION(Client, Reliable)
	void C_ActivateGem(EGearSlot GearSlot, int32 SocketIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_ReleaseGem(EGearSlot GearSlot, int32 SocketIndex);

	UFUNCTION(Client, Reliable)
	void C_ReleaseGem(EGearSlot GearSlot, int32 SocketIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void S_Damage(float Damage);

	UFUNCTION(Client, Reliable)
	void C_Transport(FVector NewLocation);

// OnRep

	UFUNCTION()
	void OnRep_Stats();

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
	USceneComponent* ProjectileSpawnComponent;

	UPROPERTY(VisibleAnywhere)
	UFighterMovementComponent* MovementComponent;

// Properties

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Stats)
	FFighterStats Stats;

	UPROPERTY(VisibleAnywhere)
	FFighterStats InitialStats;

	UPROPERTY(EditAnywhere)
	TArray<UItem*> CarriedItems;

	UPROPERTY(EditAnywhere)
	TArray<UGear*> EquippedGear;

	UPROPERTY(VisibleAnywhere)
	float GlobalCooldownRemaining;

	UPROPERTY(VisibleAnywhere)
	FVector ExternalVelocity;

	UPROPERTY(VisibleAnywhere)
	TArray<ADrop*> DropsInPickupRadius;

	UPROPERTY(VisibleAnywhere)
	bool bGrounded;

	UPROPERTY(VisibleAnywhere)
	bool bJumping;

	UPROPERTY()
	UFighterAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere)
	UGem* LastActivatedGem;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<AAbility*> ActiveAbilities;

	UPROPERTY(Replicated)
	float LookAngle;

	UPROPERTY(VisibleAnywhere)
	bool bCastLeft;

	UPROPERTY(VisibleAnywhere)
	bool bCastRight;

	FInputState PreviousInputState;

	FVector2D InputDirection;
	FVector MovementVelocity;
};

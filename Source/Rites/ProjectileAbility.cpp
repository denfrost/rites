// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileAbility.h"
#include "Fighter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

AProjectileAbility::AProjectileAbility()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionProfileName(TEXT("Projectile"));
	MeshComponent->bGenerateOverlapEvents = false;
	RootComponent = MeshComponent;

	Speed = 4000.0f;
	Gravity = -980.0f;
	Damage = 30.0f;
	MinimumDampenedDamage = 5.0f;
	Lifetime = 5.0f;
	bDampenDamageOverLifetime = true;
	BounceCount = 0;
	bPiercing = false;
	ImpactParticle = nullptr;
}

void AProjectileAbility::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileAbility::BeginOverlap);
	MeshComponent->OnComponentEndOverlap.AddDynamic(this, &AProjectileAbility::EndOverlap);
	OnDestroyed.AddDynamic(this, &AProjectileAbility::OnProjectileDestroyed);
}

void AProjectileAbility::OnProjectileDestroyed(AActor* Actor)
{
	if (ImpactParticle != nullptr)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, SpawnTransform);
	}
}

void AProjectileAbility::OnRep_InitialVelocity()
{
	Velocity = InitialVelocity;
}

void AProjectileAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Velocity -= FVector(0.0f, 0.0f, -Gravity * DeltaTime);

	FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
	FHitResult HitResult;
	SetActorLocation(NewLocation, true, &HitResult);

	if (HitResult.bBlockingHit)
	{
		// This actor hit something like a wall. Destroy it!
		Destroy();
	}
}

void AProjectileAbility::EnableOverlaps(bool bEnable)
{
	MeshComponent->bGenerateOverlapEvents = bEnable;
}

void AProjectileAbility::BeginOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AFighter* Fighter = Cast<AFighter>(OtherActor);

	if (Fighter != nullptr &&
		Fighter != Caster &&
		(bHitFriendly || !Caster->IsFriendly(Fighter)))
	{

		if (HasAuthority())
		{
			Fighter->Damage(Damage);
		}

		Destroy();
	}
}

void AProjectileAbility::EndOverlap(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AProjectileAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileAbility, Gravity);
	DOREPLIFETIME(AProjectileAbility, Damage);
	DOREPLIFETIME(AProjectileAbility, BounceCount);
	DOREPLIFETIME(AProjectileAbility, bPiercing);
	DOREPLIFETIME(AProjectileAbility, InitialVelocity);
}

void AProjectileAbility::InitDirection(FVector Direction)
{
	Direction.Normalize();
	InitialVelocity = Speed * Direction;
	Velocity = InitialVelocity;
}

void AProjectileAbility::SetCaster(AFighter* NewCaster)
{
	if (Caster != nullptr)
	{
		MeshComponent->IgnoreActorWhenMoving(Caster, false);
	}

	Super::SetCaster(NewCaster);

	if (Caster != nullptr)
	{
		MeshComponent->IgnoreActorWhenMoving(Caster, true);
	}
}

// Accessors
float AProjectileAbility::GetSpeed() const
{
	return Speed;
}

float AProjectileAbility::GetGravity() const
{
	return Gravity;
}

float AProjectileAbility::GetDamage() const
{
	return Damage;
}

float AProjectileAbility::GetMinimumDampenedDamage() const
{
	return MinimumDampenedDamage;
}

bool AProjectileAbility::GetDampenDamageOverLifetime() const
{
	return bDampenDamageOverLifetime;
}

int32 AProjectileAbility::GetBounceCount() const
{
	return BounceCount;
}

bool AProjectileAbility::IsPiercing() const
{
	return bPiercing;
}

FVector AProjectileAbility::GetVelocity() const
{
	return Velocity;
}

// Mutators

void AProjectileAbility::SetGravity(float NewGravity)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		Gravity = NewGravity;
	}
}

void AProjectileAbility::SetDamage(float NewDamage)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		Damage = NewDamage;
	}
}

void AProjectileAbility::SetBounceCount(int32 NewBounceCount)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		BounceCount = NewBounceCount;
	}
}

void AProjectileAbility::SetPiercing(bool bNewPiercing)
{
	ensure(HasAuthority());
	if (HasAuthority())
	{
		bPiercing = bNewPiercing;
	}
}
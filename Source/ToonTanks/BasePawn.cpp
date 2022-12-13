// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(
		FireRateTimeHandle,
		this,
		&ABasePawn::ClearFireRateTimer,
		FireRate,
		true,
		0.0f
	);
	
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());	
	}
	
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
	
}

float ABasePawn::GetReloadTime()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimeHandle))
	{
		return GetWorldTimerManager().GetTimerRemaining(FireRateTimeHandle);
	}
	else
	{
		return 0.0f;
	}
}

void ABasePawn::RotateTurret(FRotator Rotation)
{
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			Rotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			5.0f
		)
	);
}

void ABasePawn::AimTurret(FVector LookAtTarget)
{
	FVector ToTarget {LookAtTarget - TurretMesh->GetComponentLocation()};
	RotateTurret(FRotator(0.f, ToTarget.Rotation().Yaw, 0.f));
	
}

void ABasePawn::StraightenTurret()
{
	RotateTurret(BaseMesh->GetComponentRotation());
	
}

void ABasePawn::Fire()
{
	if (!GetWorldTimerManager().IsTimerActive(FireRateTimeHandle))
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation()
		);

		Projectile->SetOwner(this);
		SetFireRateTimer();
	}
	
}

void ABasePawn::SetFireRateTimer()
{
	GetWorldTimerManager().SetTimer(
		FireRateTimeHandle,
		this,
		&ABasePawn::ClearFireRateTimer,
		FireRate,
		true
	);
	
}

void ABasePawn::ClearFireRateTimer()
{
	GetWorldTimerManager().ClearTimer(FireRateTimeHandle);
	
}

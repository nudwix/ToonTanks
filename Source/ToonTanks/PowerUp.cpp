// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"

#include "HealthComponent.h"
#include "Tank.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
	
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnBeginOverlap);
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Float(DeltaTime);

}
void APowerUp::Float(float DeltaTime)
{
	Rotate(DeltaTime);
	Move(DeltaTime);
}

void APowerUp::Rotate(float DeltaTime)
{
	AddActorLocalRotation(RotationStep * RotationSpeed * DeltaTime);
}

void APowerUp::Move(float DeltaTime)
{
	if (ShouldReturn())
	{
		const FVector MoveDirection = MovingVelocity.GetSafeNormal();
		StartLocation += MoveDirection * MovingRange;
		SetActorLocation(StartLocation);
		MovingVelocity *= -1;
	}
	else
	{
		FVector CurrentLocation {GetActorLocation()};
		CurrentLocation += MovingVelocity * DeltaTime;
		SetActorLocation(CurrentLocation);
	}
}

bool APowerUp::ShouldReturn() const
{
	return GetDistanceMoved() > MovingRange;
}

float APowerUp::GetDistanceMoved() const
{
	return FVector::Distance(StartLocation, GetActorLocation());
}

void APowerUp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor)
	{
		ATank* Player = Cast<ATank>(OtherActor);
		if (Player)
		{
			Player->OnTakeAnyHealth.Broadcast(this, HealthAmount);
			Player->OnTakeAnyAmmo.Broadcast(this, AmmoAmount);
		}
	}
	
}
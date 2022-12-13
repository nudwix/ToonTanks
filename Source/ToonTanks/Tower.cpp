// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "PowerUp.h"
#include "Tank.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(
		FireRateTimeHandle,
		this,
		&ATower::CheckFireCondition,
		FireRate,
		true
	);
	
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (InFireRange())
	{
		AimTurret(Tank->GetActorLocation());
	}
	else
	{
		StraightenTurret();
	}

}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	DropPowerUp();
	Destroy();
	
}

void ATower::CheckFireCondition()
{
	if(Tank)
	{
		if (InFireRange() && TargetIsVisible() && Tank->bAlive)
		{
			Fire();
		}
	}
	
}

bool ATower::InFireRange() const
{
	if (Tank)
	{
		return FireRange >= FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
	}
	
	return false;
	
}

bool ATower::TargetIsVisible() const
{
	if (Tank)
	{
		FHitResult Target;
		GetWorld()->SweepSingleByChannel(
			Target,
			GetActorLocation(),
			Tank->GetActorLocation(),
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(1.0f)
		);

		return Tank == Target.GetActor();
	}

	return false;
	
}

void ATower::DropPowerUp()
{
	float DropValue = FMath::RandRange(0.0f, 100.0f);
	
	if (DropValue >= 100.0f - DropRate)
	{
		float TotalTickets {0.0f};
		for (TTuple<TSubclassOf<APowerUp>, float> PowerUpClass : PowerUpClasses)
		{
			TotalTickets += PowerUpClass.Value;
		}

		float GotTickets = FMath::RandRange(0.0f, TotalTickets);

		for (TTuple<TSubclassOf<APowerUp>, float> PowerUpClass : PowerUpClasses)
		{
			if (GotTickets <= PowerUpClass.Value)
			{
				GetWorld()->SpawnActor<APowerUp>(
					PowerUpClass.Key,
					GetActorLocation(),
					GetActorRotation()
				);
				
				return;
			}

			GotTickets -= PowerUpClass.Value;
		}
	}
}

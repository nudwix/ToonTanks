// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "Tank.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	ATank* Player = Cast<ATank>(GetOwner());
	if (Player)
	{
		Player->OnTakeAnyHealth.AddDynamic(this, &UHealthComponent::HealingTaken);	
	}
	
	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* Instigator, AActor* DamageCauser)
{
	if (Damage > 0.0f)
	{
		Health -= Damage;

		if (Health <= 0.0f)
		{
			if (ToonTanksGameMode) ToonTanksGameMode->ActorDied(DamagedActor);
		}
	}
	
}

void UHealthComponent::HealingTaken(AActor* Healer, float Healing)
{
	if (Health < MaxHealth && Healing > 0.0f)
	{
		Health += Healing;
		Healer->Destroy();
	}
	
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UHealthComponent::GetCurrentHealth() const
{
	return  Health;
	
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

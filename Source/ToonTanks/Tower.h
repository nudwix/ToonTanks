// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

class ATank;
class APowerUp;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

private:
	ATank* Tank;
	FTimerHandle FireRateTimeHandle;
	float FireRate {2.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRange {1000.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	float DropRate {25.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TMap<TSubclassOf<APowerUp>, float> PowerUpClasses;

	void CheckFireCondition();
	bool InFireRange() const;
	bool TargetIsVisible() const;
	void DropPowerUp();
	
};

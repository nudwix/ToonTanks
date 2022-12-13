// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

class UCapsuleComponent;

UCLASS()
class TOONTANKS_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float DropChance {25.0};

private:
	FVector StartLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	FCollisionProfileName CollisionProfile;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FRotator RotationStep {0.0f, 15.0f, 0.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RotationSpeed {1.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	FVector MovingVelocity {0.0f, 0.0f, 5.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovingRange {15.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float HealthAmount {50.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int AmmoAmount {0};

	void Float(float DeltaTime);
	void Rotate(float DeltaTime);
	void Move(float DeltaTime);
	bool ShouldReturn() const;
	float GetDistanceMoved() const;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

};

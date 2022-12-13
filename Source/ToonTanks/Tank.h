// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeAnyHealth, AActor*, Healer, float, AmountHealed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeAnyAmmo, AActor*, Dealer, int, AmountDealed);

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintGetter)
	int32 GetRemainingRockets() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void RocketFire();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RocketSpawnPoint;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed {300.0f};

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed {100.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> RocketClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    int32 MaxRockets {3};

	UPROPERTY(BlueprintGetter=GetRemainingRockets)
    int32 CurrentRockets;

	void Move(float Value);
	void Turn(float Value);
	void PauseMenu();

	APlayerController* TankPlayerController;

	UFUNCTION()
	void AmmoTaken(AActor* Dealer, int AmmoAmount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	bool bAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EventDispatchers")
	FOnTakeAnyHealth OnTakeAnyHealth;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EventDispatchers")
    FOnTakeAnyAmmo OnTakeAnyAmmo;
	
};

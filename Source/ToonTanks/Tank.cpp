// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Projectile.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	RocketSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Rocket Spawn Point"));
	RocketSpawnPoint->SetupAttachment(TurretMesh);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());

	CurrentRockets = MaxRockets;

	this->OnTakeAnyAmmo.AddDynamic(this, &ATank::AmmoTaken);
	
}

void ATank::AmmoTaken(AActor* Dealer, int AmmoAmount)
{
	if (CurrentRockets < MaxRockets && AmmoAmount > 0)
	{
		CurrentRockets += AmmoAmount;
		Dealer->Destroy();
	}
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		AimTurret(HitResult.ImpactPoint);
	}

}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	
	PlayerInputComponent->BindAction(TEXT("PrimaryFire"), IE_Pressed, this, &ATank::Fire);
	PlayerInputComponent->BindAction(TEXT("SecondaryFire"), IE_Pressed, this, &ATank::RocketFire);
	PlayerInputComponent->BindAction(TEXT("PauseMenu"), IE_Pressed, this, &ATank::PauseMenu);

}

APlayerController* ATank::GetPlayerController() const
{
	return TankPlayerController;
	
}

int32 ATank::GetRemainingRockets() const
{
	return CurrentRockets;
}

void ATank::Move(float Value)
{
	FVector DeltaLocation {FVector::ZeroVector};

	// X = Axis Value * Movement Speed * Delta Time
	DeltaLocation.X = Value * MovementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
	
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation {FRotator::ZeroRotator};

	// Yaw = Axis Value * Turn Speed * Delta Time
	DeltaRotation.Yaw = Value * TurnSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
	
}

void ATank::PauseMenu()
{
	if(PauseMenuClass)
	{
		UUserWidget* PauseMenuWidget = CreateWidget<UUserWidget>(this->GetGameInstance(), PauseMenuClass);
		TankPlayerController->SetInputMode(FInputModeUIOnly());
		TankPlayerController->SetPause(true);
		PauseMenuWidget->AddToViewport();
	}
}

void ATank::RocketFire()
{
	if (CurrentRockets > 0)
	{
		AProjectile* Rocket = GetWorld()->SpawnActor<AProjectile>(
			RocketClass,
			RocketSpawnPoint->GetComponentLocation(),
			RocketSpawnPoint->GetComponentRotation()
		);

		Rocket->SetOwner(this);
		CurrentRockets--;
	}
	
}
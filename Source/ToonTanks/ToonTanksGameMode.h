// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

class ATank;
class AToonTanksPlayerController;

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);
	
public:
	void ActorDied(AActor* DeadActor);

private:
	ATank* Tank;
	AToonTanksPlayerController* ToonTanksPlayerController;
	float StartDelay {3.0f};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 TargetTowers {0};

	void HandleGameStart();
	int32 GetTargetTowerCount() const;
	
};

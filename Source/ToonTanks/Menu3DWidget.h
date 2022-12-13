// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu3DWidget.generated.h"

class AMenu3D;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransitionToLinkedMenuDelegate, int32, MenuIndex);

UCLASS()
class TOONTANKS_API UMenu3DWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EventDispatchers")
	FTransitionToLinkedMenuDelegate TransitionToLinkedMenu;
	
};

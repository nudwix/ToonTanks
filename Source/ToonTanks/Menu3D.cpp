// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu3D.h"

#include "Menu3DWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMenu3D::AMenu3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMenu3D::BeginPlay()
{
	Super::BeginPlay();

	UMenu3DWidget* Menu3DWidget = Cast<UMenu3DWidget>(WidgetComponent->GetUserWidgetObject());
	if (Menu3DWidget)
	{
		Menu3DWidget->TransitionToLinkedMenu.AddDynamic(this, &AMenu3D::OnTransitionBetweenMenus);
		UE_LOG(LogTemp, Display, TEXT("Menu3DWidget is set"));
	}
	
}

// Called every frame
void AMenu3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMenu3D::TransitionTo(AMenu3D* Next)
{
	UE_LOG(LogTemp, Display, TEXT("Started transition"));
	SetActorEnableCollision(false);
	Next->SetActorEnableCollision(true);
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(Next, 2.0f, VTBlend_EaseInOut, 2.0f);
	
}

void AMenu3D::OnTransitionBetweenMenus(int32 MenuIndex)
{
	if (LinkedMenus.IsValidIndex(MenuIndex))
	{
		UE_LOG(LogTemp, Display, TEXT("Got event with men index"));
		TransitionTo(LinkedMenus[MenuIndex]);
	}
	
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "InputCoreTypes.h"

#include "UIPointerController.h"

AUIPointerController::AUIPointerController() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MotionController->SetTrackingSource(EControllerHand::Right);
	MotionController->SetShowDeviceModel(true);

	Pointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Pointer"));
	Pointer->SetupAttachment(GetRootComponent());
}

void AUIPointerController::RightTriggerPressed()
{
	Pointer->PressPointerKey(EKeys::LeftMouseButton);
}

void AUIPointerController::RightTriggerReleased()
{
	Pointer->ReleasePointerKey(EKeys::LeftMouseButton);
}

// Called when the game starts or when spawned
void AUIPointerController::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUIPointerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

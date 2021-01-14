// Fill out your copyright notice in the Description page of Project Settings.

#include "InputCoreTypes.h"

#include "UIPointerController.h"

AUIPointerController::AUIPointerController() 
{
	UE_LOG(LogTemp, Log, TEXT("Created"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MotionController->SetTrackingSource(EControllerHand::Right);
	MotionController->SetShowDeviceModel(true);

	//Root = CreateDefaultSubobject<MotionController>(TEXT("MotionController"));
	Pointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Pointer"));
	Pointer->SetupAttachment(GetRootComponent());
}

void AUIPointerController::PairControllers(AHandControllerBase* Controller)
{
	OtherController = Cast<AUIPointerController>(Controller);
	OtherController->OtherController = this;
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

void AUIPointerController::RightTriggerPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Clicked"));
	Pointer->PressPointerKey(EKeys::LeftMouseButton);
}

void AUIPointerController::RightTriggerReleased()
{
	UE_LOG(LogTemp, Log, TEXT("Released"));
	Pointer->ReleasePointerKey(EKeys::LeftMouseButton);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "UI/PaintingPicker/PaintingPicker.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	SetRootComponent(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	//if (HandControllerClass) 
	//{
	//	RightHandController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);
	//	RightHandController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	//}
	
	SetActorLocation(FVector(0, 0, -100));

	LeftController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);
	if (LeftController != nullptr)
	{
		LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		LeftController->SetMotionSource(FName("Left"));
		LeftController->SetOwner(this);
	}

	RightController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);
	if (RightController != nullptr)
	{
		RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		RightController->SetMotionSource(FName("Right"));
		RightController->SetOwner(this);
	}

	LeftController->PairControllers(RightController);
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Pressed, this, &AVRPawn::RightTriggerPressed);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Released, this, &AVRPawn::RightTriggerReleased);

	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &AVRPawn::MoveRightAxisInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRPawn::MoveRightAxisInput);
}

void AVRPawn::MoveRightAxisInput(float AxisValue)
{
	int32 PaginationOffset = 0;
	PaginationOffset += AxisValue > PaginationThumbstickThreshold ? 1 : 0;
	PaginationOffset += AxisValue < -PaginationThumbstickThreshold ? -1 : 0;

	if (PaginationOffset != LastPaginationOffset && PaginationOffset != 0)
	{
		UpdateCurrentPage(PaginationOffset);
	}

	LastPaginationOffset = PaginationOffset;
}

void AVRPawn::UpdateCurrentPage(int32 Offset)
{
	for (TActorIterator<APaintingPicker> PaintingPickerItr(GetWorld()); PaintingPickerItr; ++PaintingPickerItr)
	{
		PaintingPickerItr->UpdateCurrentPage(Offset);
	}
}
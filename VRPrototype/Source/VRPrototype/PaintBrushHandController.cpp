// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintBrushHandController.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

// Sets default values
APaintBrushHandController::APaintBrushHandController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MotionController->SetShowDeviceModel(true);
}

void APaintBrushHandController::PairControllers(AHandControllerBase* Controller)
{
	OtherController = Cast<APaintBrushHandController>(Controller);
	OtherController->OtherController = this;
}

void APaintBrushHandController::Grip()
{
	if (!bCanClimb) return;

	if (!bIsClimbing)
	{
		bIsClimbing = true;
		ClimbingStartLocation = GetActorLocation();

		OtherController->bIsClimbing = false;

		ACharacter* Character = Cast<ACharacter>(GetAttachParentActor());
		if (Character != nullptr)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}
}

void APaintBrushHandController::Release()
{
	if (bIsClimbing)
	{
		bIsClimbing = false;

		ACharacter* Character = Cast<ACharacter>(GetAttachParentActor());
		if (Character != nullptr)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
	}
}

void APaintBrushHandController::RightTriggerPressed()
{
	CurrentStroke = GetWorld()->SpawnActor<AStroke>(StrokeClass);
	CurrentStroke->SetActorLocation(GetActorLocation());
}

void APaintBrushHandController::RightTriggerReleased()
{
	CurrentStroke = nullptr;
}

// Called when the game starts or when spawned
void APaintBrushHandController::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APaintBrushHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APaintBrushHandController::ActorEndOverlap);
}

// Called every frame
void APaintBrushHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsClimbing)
	{
		FVector PaintBrushHandControllerDelta = GetActorLocation() - ClimbingStartLocation;
		GetAttachParentActor()->AddActorWorldOffset(-PaintBrushHandControllerDelta);
	}

	if (CurrentStroke) 
	{
		CurrentStroke->Update(GetActorLocation());
	}
}

void APaintBrushHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bool bNewCanClimb = CanClimb();
	if (!bCanClimb && bNewCanClimb)
	{
		APawn* Pawn = Cast<APawn>(GetAttachParentActor());
		if (Pawn != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
			if (PlayerController != nullptr)
			{
				EControllerHand ControllerHand = MotionController->MotionSource == FName("Left") ? EControllerHand::Left : EControllerHand::Right;
				PlayerController->PlayHapticEffect(HapticEffect, ControllerHand);
			}
		}
	}
	bCanClimb = bNewCanClimb;
}

void APaintBrushHandController::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bCanClimb = CanClimb();
}

bool APaintBrushHandController::CanClimb() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag(TEXT("Climable"))) return true;
	}

	return false;
}


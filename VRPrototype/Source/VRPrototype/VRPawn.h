// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "HandControllerBase.h"
#include "VRPawn.generated.h"

UCLASS()
class VRPROTOTYPE_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AHandControllerBase> HandControllerClass;

	// Components
	UPROPERTY()
	USceneComponent* VRRoot;

	UPROPERTY()
	UCameraComponent* Camera;

	// Referance
	UPROPERTY()
	AHandControllerBase* LeftController;
	UPROPERTY()
	AHandControllerBase* RightController;
};

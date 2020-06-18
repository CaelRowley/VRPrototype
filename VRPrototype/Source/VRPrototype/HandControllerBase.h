// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"

#include "HandControllerBase.generated.h"

UCLASS()
class VRPROTOTYPE_API AHandControllerBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AHandControllerBase();
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* MotionController;

	inline void SetMotionSource(FName MotionSource) { MotionController->SetTrackingMotionSource(MotionSource); }
	virtual void PairControllers(AHandControllerBase* Controller) { }

	virtual void Grip() { }
	virtual void Release() { }
	virtual void RightTriggerPressed() { }
	virtual void RightTriggerReleased() { }
};

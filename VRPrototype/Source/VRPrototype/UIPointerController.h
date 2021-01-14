// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "Components/WidgetInteractionComponent.h"

#include "UIPointerController.generated.h"

/**
 *
 */
UCLASS()
class VRPROTOTYPE_API AUIPointerController : public AHandControllerBase
{
	GENERATED_BODY()

public:
	AUIPointerController();

	void PairControllers(AHandControllerBase* Controller) override;
	void RightTriggerPressed() override;
	void RightTriggerReleased() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//UPROPERTY()
	//class MotionController* Root;

	AUIPointerController* OtherController;

	UPROPERTY(VisibleAnywhere)
	UWidgetInteractionComponent* Pointer;
};

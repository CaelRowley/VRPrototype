// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stroke.h"
#include "HandControllerBase.h"
#include "Components/WidgetComponent.h"

#include "PaintBrushHandController.generated.h"

UCLASS()
class VRPROTOTYPE_API APaintBrushHandController : public AHandControllerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintBrushHandController();

	void PairControllers(AHandControllerBase* Controller) override;
	void Grip() override;
	void Release() override;
	void RightTriggerPressed() override;
	void RightTriggerReleased() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Callbacks
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// Helpers
	bool CanClimb() const;

	// Default sub objects

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStroke> StrokeClass;

	// State
	bool bCanClimb = false;
	bool bIsClimbing = false;
	FVector ClimbingStartLocation;
	APaintBrushHandController* OtherController;
	AStroke* CurrentStroke;

	// Configuration Parameters
	UPROPERTY(EditAnywhere)
	class UHapticFeedbackEffect_Base* HapticEffect;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* PaletteMenu;
};

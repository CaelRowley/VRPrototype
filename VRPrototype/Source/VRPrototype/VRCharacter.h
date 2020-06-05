// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class VRPROTOTYPE_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	class UCameraComponent* Camera;
	UPROPERTY()
	class UMotionControllerComponent* LeftController;
	UPROPERTY()
	class UMotionControllerComponent* RightController;
	UPROPERTY()
	class USceneComponent* VRRoot;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;
	UPROPERTY()
	class UPostProcessComponent* PostProcessComponent;
	UPROPERTY()
	class UMaterialInstanceDynamic* BlinkerMaterialInstance;

	bool FindTeleportDestination(FVector& OutLocation);
	void StartFade(float fromAlpha, float toAlpha);
	void UpdateDestinationMarker();
	void UpdateBlinkers();
	FVector2D GetBlinkerCentre();

	void MoveForward(float throttle);
	void MoveBack(float throttle);
	void MoveLeft(float throttle);
	void MoveRight(float throttle);

	void BeginTeleport();
	void FinishTeleport(FVector destination);

	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000;
	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 0.5;
	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100, 100, 100);
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlinkerMaterialBase;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* RadiusVsVelocity;
};

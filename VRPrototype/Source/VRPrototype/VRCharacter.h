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
	class USceneComponent* VRRoot;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;

	bool FindTeleportDestination(FVector& OutLocation);
	void StartFade(float fromAlpha, float toAlpha);

	void UpdateDestinationMarker();
;	void MoveForward(float throttle);
	void MoveSide(float throttle);

	void BeginTeleport();
	void FinishTeleport();

	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000;
	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 0.5;
	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100, 100, 100);
};

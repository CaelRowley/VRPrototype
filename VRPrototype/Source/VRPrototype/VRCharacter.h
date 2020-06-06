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

	UPROPERTY()
	class AHandController* LeftController;
	UPROPERTY()
	class AHandController* RightController;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* TeleportPath;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;
	UPROPERTY()
	class UPostProcessComponent* PostProcessComponent;
	UPROPERTY()
	class UMaterialInstanceDynamic* BlinkerMaterialInstance;
	UPROPERTY()
	TArray<class USplineMeshComponent*> TeleportPathMeshPool;


	bool FindTeleportDestination(TArray<FVector> &OutPath, FVector& OutLocation);
	void StartFade(float fromAlpha, float toAlpha);
	void UpdateDestinationMarker();
	void UpdateBlinkers();
	void DrawTeleportPath(const TArray<FVector>& Path);
	void UpdateSpline(const TArray<FVector>& Path);
	FVector2D GetBlinkerCentre();

	void MoveForward(float throttle);
	void MoveBack(float throttle);
	void MoveLeft(float throttle);
	void MoveRight(float throttle);

	void BeginTeleport();
	void FinishTeleport(FVector destination);

	// Configuration Parameters
	UPROPERTY(EditAnywhere)
	float TeleportProjectileSpeed = 800;
	UPROPERTY(EditAnywhere)
	float TeleportProjectileRadius = 10;
	UPROPERTY(EditAnywhere)
	float TeleportSimultionTime = 1;
	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 0.5;
	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlinkerMaterialBase;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* RadiusVsVelocity;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* TeleportArchMesh;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TeleportArchMaterial;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> HandControllerClass;
};

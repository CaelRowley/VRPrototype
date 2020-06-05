// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MotionControllerComponent.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	LeftController->SetupAttachment(VRRoot);
	LeftController->SetTrackingMotionSource(FName("Left"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightController->SetupAttachment(VRRoot);
	RightController->SetTrackingMotionSource(FName("Right"));


	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestinationMarker"));
	Camera->SetupAttachment(GetRootComponent());

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	Camera->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = true;
	DestinationMarker->SetVisibility(false);

	BlinkerMaterialInstance = UMaterialInstanceDynamic::Create(BlinkerMaterialBase, this);
	PostProcessComponent->AddOrUpdateBlendable(BlinkerMaterialInstance);	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: should set player capsule to headset location but causes constant movement
	//FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	//NewCameraOffset.Z = 0;
	//AddActorWorldOffset(NewCameraOffset);
	//VRRoot->AddWorldOffset(-NewCameraOffset);

	UpdateDestinationMarker();
	UpdateBlinkers();
}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveBack"), this, &AVRCharacter::MoveBack);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft"), this, &AVRCharacter::MoveLeft);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AVRCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AVRCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released ,this, &AVRCharacter::BeginTeleport);
}

bool AVRCharacter::FindTeleportDestination(FVector& OutLocation)
{
	FVector Start = RightController->GetComponentLocation();
	FVector Look = RightController->GetForwardVector();
	Look = Look.RotateAngleAxis(35, RightController->GetRightVector());
	FVector End = Start + Look * MaxTeleportDistance;
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (!bHit) return false;

	FNavLocation NavLocation;
	bool bOnNavMesh = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(HitResult.Location, NavLocation, TeleportProjectionExtent);

	if (!bOnNavMesh) return false;

	OutLocation = NavLocation.Location;

	return bHit && bOnNavMesh;
}

void AVRCharacter::UpdateDestinationMarker()
{
	FVector Location;
	bool bHasDestination = FindTeleportDestination(Location);

	DestinationMarker->SetVisibility(bHasDestination);
	DestinationMarker->SetWorldLocation(Location);
}

void AVRCharacter::UpdateBlinkers()
{
	float Speed = GetVelocity().Size();
	float Radius = RadiusVsVelocity->GetFloatValue(Speed);
	BlinkerMaterialInstance->SetScalarParameterValue(TEXT("Radius"), Radius);

	FVector2D Centre = GetBlinkerCentre();
	BlinkerMaterialInstance->SetVectorParameterValue(TEXT("Centre"), FLinearColor(Centre.X, Centre.Y, 0));
}

FVector2D AVRCharacter::GetBlinkerCentre()
{
	FVector MoveDirection = GetVelocity().GetSafeNormal();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (MoveDirection.IsNearlyZero() || PlayerController == nullptr) {
		return FVector2D(0.5, 0.5);
	}


	if (FVector::DotProduct(Camera->GetForwardVector(), MoveDirection) <= 0) MoveDirection *= -1;
	FVector WorldStationaryLocation = Camera->GetComponentLocation() + MoveDirection * 1000;

	FVector2D ScreenStationaryLocation;
	PlayerController->ProjectWorldLocationToScreen(WorldStationaryLocation, ScreenStationaryLocation, true);

	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);
	ScreenStationaryLocation.X /= SizeX;
	ScreenStationaryLocation.Y /= SizeY;

	return ScreenStationaryLocation;
}

void AVRCharacter::MoveForward(float throttle)
{
	if (throttle != 0) {
		UE_LOG(LogTemp, Warning, TEXT("MoveForward: %f"), throttle);
	}
	AddMovementInput(throttle * Camera->GetForwardVector());
}

void AVRCharacter::MoveBack(float throttle)
{
	if (throttle != 0) {
		UE_LOG(LogTemp, Warning, TEXT("MoveBack: %f"), throttle);
	}
	AddMovementInput(throttle * Camera->GetForwardVector());
}

void AVRCharacter::MoveLeft(float throttle)
{
	if (throttle != 0) {
		UE_LOG(LogTemp, Warning, TEXT("MoveRight: %f"), throttle);
	}
	AddMovementInput(throttle * Camera->GetRightVector());
}

void AVRCharacter::MoveRight(float throttle)
{
	if (throttle != 0) {
		UE_LOG(LogTemp, Warning, TEXT("MoveRight: %f"), throttle);
	}
	AddMovementInput(throttle * Camera->GetRightVector());
}


void AVRCharacter::BeginTeleport()
{
	if (!DestinationMarker->IsVisible()) return;

	StartFade(0, 1);
	FTimerHandle Handle;
	FTimerDelegate FinishTeleportDelegate = FTimerDelegate::CreateUObject(this, &AVRCharacter::FinishTeleport, DestinationMarker->GetComponentLocation());
	GetWorldTimerManager().SetTimer(Handle, FinishTeleportDelegate, TeleportFadeTime, false);
}

void AVRCharacter::FinishTeleport(FVector destination)
{
	SetActorLocation(destination + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	StartFade(1, 0);
}

void AVRCharacter::StartFade(float fromAlpha, float toAlpha)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		PlayerController->PlayerCameraManager->StartCameraFade(fromAlpha, toAlpha, TeleportFadeTime, FLinearColor::Black);
	}
}
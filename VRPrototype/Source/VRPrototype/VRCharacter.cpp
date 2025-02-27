// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacter.h"

#include "Saves/PainterSaveGame.h"
#include "VRGameMode.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NavigationSystem.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("TeleportPath"));
	TeleportPath->SetupAttachment(VRRoot);

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

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Released, this, &AVRCharacter::BeginTeleport);

	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Pressed, this, &AVRCharacter::GripLeft);
	PlayerInputComponent->BindAction(TEXT("GripLeft"), IE_Released, this, &AVRCharacter::ReleaseLeft);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Pressed, this, &AVRCharacter::GripRight);
	PlayerInputComponent->BindAction(TEXT("GripRight"), IE_Released, this, &AVRCharacter::ReleaseRight);

	//PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Pressed, this, &AVRCharacter::LeftTriggerPressed);
	//PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Released, this, &AVRCharacter::LeftTriggerReleased);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Pressed, this, &AVRCharacter::RightTriggerPressed);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Released, this, &AVRCharacter::RightTriggerReleased);

	PlayerInputComponent->BindAction(TEXT("Save"), IE_Released, this, &AVRCharacter::Save);
	PlayerInputComponent->BindAction(TEXT("Menu"), IE_Released, this, &AVRCharacter::Menu);
}

bool AVRCharacter::FindTeleportDestination(TArray<FVector>& OutPath, FVector& OutLocation)
{
	FVector Start = LeftController->GetActorLocation();
	FVector Look = LeftController->GetActorForwardVector();

	FPredictProjectilePathParams PathParams(
		TeleportProjectileRadius,
		Start,
		Look * TeleportProjectileSpeed,
		TeleportSimultionTime,
		ECollisionChannel::ECC_Visibility,
		this
	);
	PathParams.bTraceComplex = true;

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);

	bool bHit = UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);

	if (!bHit) return false;

	for (FPredictProjectilePathPointData PointData : PathResult.PathData) { OutPath.Add(PointData.Location); }

	FNavLocation NavLocation;
	bool bOnNavMesh = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(PathResult.HitResult.Location, NavLocation, TeleportProjectionExtent);

	if (!bOnNavMesh) return false;

	OutLocation = NavLocation.Location;

	return bHit && bOnNavMesh;
}

void AVRCharacter::UpdateDestinationMarker()
{
	TArray<FVector> Path;
	FVector Location;
	bool bHasDestination = FindTeleportDestination(Path, Location);

	DestinationMarker->SetVisibility(bHasDestination);
	DestinationMarker->SetWorldLocation(Location);
	TArray<FVector> EmptyPath;
	DrawTeleportPath(bHasDestination ? Path : EmptyPath);
}

void AVRCharacter::UpdateBlinkers()
{
	float Speed = GetVelocity().Size();
	float Radius = RadiusVsVelocity->GetFloatValue(Speed);
	BlinkerMaterialInstance->SetScalarParameterValue(TEXT("Radius"), Radius);

	FVector2D Centre = GetBlinkerCentre();
	BlinkerMaterialInstance->SetVectorParameterValue(TEXT("Centre"), FLinearColor(Centre.X, Centre.Y, 0));
}

void AVRCharacter::DrawTeleportPath(const TArray<FVector>& Path)
{
	UpdateSpline(Path);

	for (USplineMeshComponent* SplineMesh : TeleportPathMeshPool)
	{
		SplineMesh->SetVisibility(false);
	}

	for (int32 i = 0; i < Path.Num() - 1; i++)
	{
		if (TeleportPathMeshPool.Num() <= i)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(TeleportArchMesh);
			SplineMesh->SetMaterial(0, TeleportArchMaterial);
			SplineMesh->RegisterComponent();

			TeleportPathMeshPool.Add(SplineMesh);
		}

		USplineMeshComponent* SplineMesh = TeleportPathMeshPool[i];
		SplineMesh->SetVisibility(true);

		FVector StartPos, StartTangent, EndPos, EndTangent;
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i, StartPos, StartTangent);
		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent);

		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
	}
}

void AVRCharacter::UpdateSpline(const TArray<FVector>& Path)
{
	TeleportPath->ClearSplinePoints(false);
	for (int32 i = 0; i < Path.Num(); i++)
	{
		FVector LocalPosition = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
		FSplinePoint SplinePoint(i, LocalPosition, ESplinePointType::Curve);
		TeleportPath->AddPoint(SplinePoint, false);
	}

	TeleportPath->UpdateSpline();
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
	AddMovementInput(throttle * Camera->GetForwardVector());
}

void AVRCharacter::MoveBack(float throttle)
{
	AddMovementInput(throttle * Camera->GetForwardVector());
}

void AVRCharacter::MoveLeft(float throttle)
{
	AddMovementInput(throttle * Camera->GetRightVector());
}

void AVRCharacter::MoveRight(float throttle)
{
	AddMovementInput(throttle * Camera->GetRightVector());
}

void AVRCharacter::Save()
{
	//auto GameMode = Cast<AVRGameMode>(GetWorld()->GetAuthGameMode());
	//if (!GameMode) return;
	//GameMode->Save();

	// UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void AVRCharacter::Menu()
{
	//auto GameMode = Cast<AVRGameMode>(GetWorld()->GetAuthGameMode());
	//if (!GameMode) return;
	//GameMode->Load();
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
	SetActorLocation(destination + (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * GetActorUpVector()));
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
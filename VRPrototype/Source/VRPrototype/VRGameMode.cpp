// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGameMode.h"

#include "Saves/PainterSaveGame.h"

#include "Kismet/StereoLayerFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

void AVRGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	SlotName = UGameplayStatics::ParseOption(Options, "SlotName");

	UE_LOG(LogTemp, Warning, TEXT("SlotName: %s"), *SlotName);
}

void AVRGameMode::BeginPlay()
{
	Super::BeginPlay();

	UPainterSaveGame* SaveGame = UPainterSaveGame::Load(SlotName);
	if (SaveGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Save: %s"), *SlotName);
		SaveGame->DeserializeToWorld(GetWorld());

		UStereoLayerFunctionLibrary::HideSplashScreen();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save not found: %s"), *SlotName);
	}
}
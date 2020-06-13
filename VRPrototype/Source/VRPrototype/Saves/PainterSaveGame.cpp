// Fill out your copyright notice in the Description page of Project Settings.


#include "PainterSaveGame.h"

#include "../Stroke.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

UPainterSaveGame* UPainterSaveGame::Create()
{
	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(StaticClass());
	return Cast<UPainterSaveGame>(NewSaveGame);
}

UPainterSaveGame* UPainterSaveGame::Load()
{
	return Cast<UPainterSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Test"), 0));
}

bool UPainterSaveGame::Save()
{
	return UGameplayStatics::SaveGameToSlot(this, TEXT("Test"), 0);
}

void UPainterSaveGame::SerializeFromWorld(UWorld* World)
{
	// TODO: Serialize strokes

	Strokes.Empty();

	for (TActorIterator<AStroke> StrokeIterator(World); StrokeIterator; ++StrokeIterator)
	{
		Strokes.Add(StrokeIterator->GetClass());
	}

}

void UPainterSaveGame::DeserializeToWorld(UWorld* World)
{
	// TODO: Spawn all serialized strokes
	ClearWorld(World);

	for (TSubclassOf<class AStroke> Stroke : Strokes)
	{
		World->SpawnActor<AStroke>(Stroke);
	}
}

void UPainterSaveGame::ClearWorld(UWorld* World)
{
	for (TActorIterator<AStroke> StrokeIterator(World); StrokeIterator; ++StrokeIterator)
	{
		StrokeIterator->Destroy();
	}
}

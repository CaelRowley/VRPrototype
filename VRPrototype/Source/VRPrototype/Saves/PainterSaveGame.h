// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PainterSaveGame.generated.h"

USTRUCT()
struct FStrokeState
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<class AStroke> Class;
	UPROPERTY()
	TArray<FVector> ControlPoints;
};

UCLASS()
class VRPROTOTYPE_API UPainterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static UPainterSaveGame* Create();
	static UPainterSaveGame* Load(FString SlotNames);
	bool Save();

	inline void SetState(FString NewState) { State = NewState; }
	inline FString GetState() const { return State; }

	void SerializeFromWorld(UWorld* World);
	void DeserializeToWorld(UWorld* World);

	FString GetSlotNames() const { return SlotNames; }

private:
	// State
	UPROPERTY()
	FString State;

	UPROPERTY()
	FString SlotNames;

	UPROPERTY()
	TArray<FStrokeState> Strokes;

	void ClearWorld(UWorld* World);
};

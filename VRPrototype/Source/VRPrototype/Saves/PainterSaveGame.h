// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PainterSaveGame.generated.h"

UCLASS()
class VRPROTOTYPE_API UPainterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static UPainterSaveGame* Create();
	static UPainterSaveGame* Load();
	bool Save();

	inline void SetState(FString NewState) { State = NewState; }
	inline FString GetState() const { return State; }

	void SerializeFromWorld(UWorld* World);
	void DeserializeToWorld(UWorld* World);

private:
	// State
	UPROPERTY()
	FString State;

	UPROPERTY()
	TArray<TSubclassOf<class AStroke>> Strokes;

	void ClearWorld(UWorld* World);
};

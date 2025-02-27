// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Saves/PainterSaveGame.h"

#include "Stroke.generated.h"

UCLASS()
class VRPROTOTYPE_API AStroke : public AActor
{
	GENERATED_BODY()
	
public:	
	AStroke();

	void Update(FVector CursorLocation);
	AStroke* CurrentStroke;

	FStrokeState SerializeToStruct() const;
	static AStroke* DeserializeFromStruct(UWorld* World, const FStrokeState& StrokeState);

private:
	FTransform GetNextSegmentTransform(FVector CurrentCursorLocation) const;
	FTransform GetNextJointTransform(FVector CurrentCursorLocation) const;

	FVector GetNextSegmentScale(FVector CurrentCursorLocation) const;
	FQuat GetNextSegmentRotation(FVector CurrentCursorLocation) const;
	FVector GetNextSegmentLocation(FVector CurrentCursorLocation) const;

	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* StrokeMeshes;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* JointMeshes;

	// State
	FVector PreviousCursorLocation;
	TArray<FVector> ControlPoints;
};

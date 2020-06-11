// Fill out your copyright notice in the Description page of Project Settings.


#include "Stroke.h"

#include "Components/SplineMeshComponent.h"

// Sets default values
AStroke::AStroke()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StrokeMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StrokeMeshes"));
	StrokeMeshes->SetupAttachment(Root);	
	JointMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("JointMeshes"));
	JointMeshes->SetupAttachment(Root);
}

void AStroke::Update(FVector CursorLocation)
{
	if (PreviousCursorLocation.IsNearlyZero())
	{
		PreviousCursorLocation = CursorLocation;
		JointMeshes->AddInstance(GetNextJointTransform(CursorLocation));
		return;
	}

	StrokeMeshes->AddInstance(GetNextSegmentTransform(CursorLocation));
	JointMeshes->AddInstance(GetNextJointTransform(CursorLocation));

	PreviousCursorLocation = CursorLocation;
}

FTransform AStroke::GetNextSegmentTransform(FVector CurrentCursorLocation) const
{
	FTransform SegmentTransform;

	SegmentTransform.SetScale3D(GetNextSegmentScale(CurrentCursorLocation));
	SegmentTransform.SetRotation(GetNextSegmentRotation(CurrentCursorLocation));
	SegmentTransform.SetLocation(GetNextSegmentLocation(CurrentCursorLocation));

	return SegmentTransform;
}

FTransform AStroke::GetNextJointTransform(FVector CurrentCursorLocation) const
{
	FTransform JointTransform;
	JointTransform.SetLocation(GetTransform().InverseTransformPosition(CurrentCursorLocation));
;	return JointTransform;
}

FVector AStroke::GetNextSegmentScale(FVector CurrentCursorLocation) const
{
	FVector Segment = CurrentCursorLocation - PreviousCursorLocation;
	return FVector(Segment.Size(), 1, 1);
}

FQuat AStroke::GetNextSegmentRotation(FVector CurrentCursorLocation) const
{
	FVector Segment = CurrentCursorLocation - PreviousCursorLocation;
	FVector SegmentNormal = Segment.GetSafeNormal();
	return FQuat::FindBetweenNormals(FVector::ForwardVector, SegmentNormal);
}

FVector AStroke::GetNextSegmentLocation(FVector CurrentCursorLocation) const
{
	return GetTransform().InverseTransformPosition(PreviousCursorLocation);
}





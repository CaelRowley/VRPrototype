// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingGrid.h"

#include "Components/SizeBox.h"

void UPaintingGrid::AddPainting(int32 PaintingIndex, FString PaintingName)
{
	if (!PaintingGrid) return;

	UE_LOG(LogTemp, Warning, TEXT("Grid Name: %s"), *PaintingGrid->GetName());
	UPaintingGridCard* NewWidget = CreateWidget<UPaintingGridCard>(GetWorld(), GridCardClass);

	if (!NewWidget) return;

	NewWidget->SetPaitningName(PaintingName);
	USizeBox* CardContainer = Cast<USizeBox>(PaintingGrid->GetChildAt(PaintingIndex));

	if (!CardContainer) return;

	CardContainer->AddChild(NewWidget);
}

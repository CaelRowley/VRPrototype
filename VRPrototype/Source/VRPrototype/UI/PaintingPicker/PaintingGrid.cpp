// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingGrid.h"

#include "Components/SizeBox.h"
#include "Components/HorizontalBoxSlot.h"

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

void UPaintingGrid::AddPaginationDot(bool Active)
{
	if (!PaginationDots) return;

	auto Dot = CreateWidget<UPaginationDot>(GetWorld(), PaginationDotClass);
	if (!Dot) return;

	Dot->SetActive(Active);

	UHorizontalBoxSlot* DotSlot = PaginationDots->AddChildToHorizontalBox(Dot);
	DotSlot->SetPadding(FMargin(PaginationDotPadding, 0));
}

void UPaintingGrid::ClearPaintings()
{
	for (int32 i = 0; i < PaintingGrid->GetChildrenCount(); i++)
	{
		USizeBox* CardContainer = Cast<USizeBox>(PaintingGrid->GetChildAt(i));
		if (!CardContainer) continue;

		CardContainer->ClearChildren();
	}
}

int32 UPaintingGrid::GetNumberOfSlots() const
{
	return PaintingGrid->GetChildrenCount();
}

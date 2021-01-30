// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingPicker.h"

#include "../../Saves/PainterSaveGameIndex.h"
#include "../../Saves/PainterSaveGame.h"

#include "PaintingGrid.h"
#include "ActionBar.h"

// Sets default values
APaintingPicker::APaintingPicker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PaintingGrid = CreateDefaultSubobject<UWidgetComponent>(TEXT("PaintingGrid"));
	PaintingGrid->SetupAttachment(GetRootComponent());

	ActionBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActionBar"));
	ActionBar->SetupAttachment(GetRootComponent());
}

void APaintingPicker::AddPainting()
{
	UE_LOG(LogTemp, Warning, TEXT("Add Button Clicked"));
	UPainterSaveGame::Create();

	RefreshSlots();
}

void APaintingPicker::ToggleDeleteMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Delete Button Clicked"));


	UPaintingGrid* PaintingGridWidget = Cast<UPaintingGrid>(PaintingGrid->GetUserWidgetObject());
	if (!PaintingGridWidget) return;

	PaintingGridWidget->ClearPaintings();
}

// Called when the game starts or when spawned
void APaintingPicker::BeginPlay()
{
	Super::BeginPlay();

	UActionBar* ActionBarWidget = Cast<UActionBar>(ActionBar->GetUserWidgetObject());
	if (ActionBarWidget)
	{
		ActionBarWidget->SetParentPicker(this);
	}

	RefreshSlots();
}

void APaintingPicker::RefreshSlots()
{
	UPaintingGrid* PaintingGridWidget = Cast<UPaintingGrid>(PaintingGrid->GetUserWidgetObject());
	if (!PaintingGridWidget) return;

	PaintingGridWidget->ClearPaintings();

	int32 Index = 0;
	for (FString SlotName : UPainterSaveGameIndex::Load()->GetSlotNames())
	{
		PaintingGridWidget->AddPainting(Index, SlotName);
		++Index;
		UE_LOG(LogTemp, Warning, TEXT("Painting name: %s"), *SlotName);
	}
}


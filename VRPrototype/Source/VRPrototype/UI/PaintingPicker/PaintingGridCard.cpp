// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingGridCard.h"

#include "Kismet/StereoLayerFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UPaintingGridCard::SetPaitningName(FString NewPaintingName)
{
	PaintingName = NewPaintingName;
	SlotName->SetText(FText::FromString(PaintingName));

	CardButton->OnClicked.AddDynamic(this, &UPaintingGridCard::CardButtonClicked);
}

void UPaintingGridCard::CardButtonClicked()
{
	UStereoLayerFunctionLibrary::ShowSplashScreen();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"), true, "SlotName=" + PaintingName);
}

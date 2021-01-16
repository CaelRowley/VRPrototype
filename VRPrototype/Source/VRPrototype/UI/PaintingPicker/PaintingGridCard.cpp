// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingGridCard.h"

void UPaintingGridCard::SetPaitningName(FString PaintingName)
{
	SlotName->SetText(FText::FromString(PaintingName));
}

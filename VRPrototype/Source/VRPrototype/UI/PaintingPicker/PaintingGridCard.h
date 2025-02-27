// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "PaintingGridCard.generated.h"

/**
 * 
 */
UCLASS()
class VRPROTOTYPE_API UPaintingGridCard : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPaitningName(FString PaintingName);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UTextBlock* SlotName;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget))
	UButton* CardButton;

private:
	UFUNCTION()
	void CardButtonClicked();

	FString PaintingName;
};

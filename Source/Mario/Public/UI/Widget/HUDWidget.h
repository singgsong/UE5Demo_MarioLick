// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MARIO_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealth(int32 Value);
	void UpdateCoins(int32 Value);
	void UpdatePoints(int32 Value);
	void UpdateTime(int32 Value);
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Coins;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Points;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Time;

};


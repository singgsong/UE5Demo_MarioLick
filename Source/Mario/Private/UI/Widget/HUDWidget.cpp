// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/HUDWidget.h"
#include "Components/TextBlock.h"

void UHUDWidget::UpdateHealth(int32 Value)
{
	if (Health)
		Health->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
}

void UHUDWidget::UpdateCoins(int32 Value)
{
	if (Coins)
		Coins->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
}

void UHUDWidget::UpdatePoints(int32 Value)
{
	if (Points)
		Points->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
}

void UHUDWidget::UpdateTime(int32 Value)
{
	if (Time)
		Time->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
}

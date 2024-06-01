// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MarioHUD.h"
#include "UI/Widget/HUDWidget.h"

void AMarioHUD::BeginPlay()
{
	Super::BeginPlay();
	if (HUDWidgetClass)
	{
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::MakeRandomColor(), FString::Printf(TEXT("HUDWidgetClass未设置")));
	}

}

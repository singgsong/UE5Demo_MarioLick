// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Component/PointRewardComponent.h"
#include "UI/Widget/PointsRewardWidget.h"

void UPointRewardComponent::BeginPlay()
{
	Super::BeginPlay();
	PointsRewardWidget = Cast<UPointsRewardWidget>(GetWidget());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetWidget()->GetName());

}

void UPointRewardComponent::ShowPoints(int32 Value)
{
	if (PointsRewardWidget)
	{
		PointsRewardWidget->UpdatePoints(Value);
		PointsRewardWidget->PlayRewardAnim();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PointsRewardWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UPointsRewardWidget::UpdatePoints(int32 Value)
{
	if (Points) 
		Points->SetText(FText::FromString(FString::Printf(TEXT("%d"), Value)));
}

void UPointsRewardWidget::PlayRewardAnim()
{
	if (RewardAnim)
		PlayAnimation(RewardAnim);
}

float UPointsRewardWidget::AnimEnd()
{
	return RewardAnim->GetEndTime();
}

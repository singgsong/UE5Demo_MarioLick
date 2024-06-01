// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointsRewardWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class MARIO_API UPointsRewardWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdatePoints(int32 Value);
	void PlayRewardAnim();
	float AnimEnd();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Points;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardAnim;

};

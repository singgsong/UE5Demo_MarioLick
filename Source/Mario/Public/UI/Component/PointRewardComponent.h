// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PointRewardComponent.generated.h"

class UPointsRewardWidget;

/**
 * 
 */
UCLASS()
class MARIO_API UPointRewardComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowPoints(int32 Value);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointsRewardWidget> PointsRewardWidget;

private:

};

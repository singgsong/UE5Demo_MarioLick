// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockBase.h"
#include "PointsReward.generated.h"

class UPointRewardComponent;

/**
 *
 */
UCLASS()
class MARIO_API APointsReward : public AActor
{
	GENERATED_BODY()
	
public:	
	APointsReward();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPointRewardComponent> PointRewardComponent;

	UPROPERTY(EditDefaultsOnly)
	float AnimTime = 1.f;

	UPROPERTY(VisibleAnywhere)
	int32 Points = 0;

public:
	FORCEINLINE void SetPoints(int32 Value) { Points = Value; }

};

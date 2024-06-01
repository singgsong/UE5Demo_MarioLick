// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointsReward.h"
#include "UI/Component/PointRewardComponent.h"
#include "UI/Widget/PointsRewardWidget.h"

APointsReward::APointsReward()
{
	PrimaryActorTick.bCanEverTick = false;
	PointRewardComponent = CreateDefaultSubobject<UPointRewardComponent>(TEXT("PointRewardComponent"));
	SetRootComponent(PointRewardComponent);
}

void APointsReward::BeginPlay()
{
	Super::BeginPlay();

	PointRewardComponent->ShowPoints(Points);

	FTimerDelegate TimerDelegate;
	FTimerHandle TimeToDestroy;
	TimerDelegate.BindLambda([this]() { Destroy(); });

	GetWorldTimerManager().SetTimer(TimeToDestroy,TimerDelegate, AnimTime, false);
}



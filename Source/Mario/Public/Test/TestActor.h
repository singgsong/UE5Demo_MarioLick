// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TestActor.generated.h"

class UTimelineComponent;

UCLASS()
class MARIO_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestActor();

protected:
	virtual void BeginPlay() override;
	void Float1Func(float Value);
	void Float2Func(float Value);
	void FinishFunc();

	UFUNCTION()
	void UpdateFunc();
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> Timeline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> FloatCurve1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> FloatCurve2;

	FOnTimelineEventStatic FinishEvent;
	FOnTimelineEvent UpdateFuncEvent;

	FOnTimelineFloatStatic FloatEvent1;
	FOnTimelineFloatStatic FloatEvent2;

};

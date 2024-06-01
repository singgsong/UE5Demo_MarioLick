// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AMarioController;
/**
 *
 */
UCLASS()
class MARIO_API ABaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseActor();

protected:
	virtual void BeginPlay() override;

	TObjectPtr<AMarioController> MarioController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> ActorRootComponent; // 为了能方便使用SetRelativeLocation而加的，作为Actor的根组件

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	#pragma region Timeline
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY(EditAnywhere)
	bool TimelineLoop = false;
	#pragma endregion

};

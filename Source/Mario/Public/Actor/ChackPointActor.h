// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "ChackPointActor.generated.h"

/**
 * 
 */
UCLASS()
class MARIO_API AChackPointActor : public ABaseActor
{
	GENERATED_BODY()
public:
	AChackPointActor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> ActivatedMaterial;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Activated;

	
};

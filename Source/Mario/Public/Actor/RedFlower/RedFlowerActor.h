// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "RedFlowerActor.generated.h"

/**
 * 
 */
UCLASS()
class MARIO_API ARedFlowerActor : public ABaseActor
{
	GENERATED_BODY()
public:
	ARedFlowerActor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void FlowerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> SpawnFlowerCurve;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> VFX_Flower;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_PlayerPowerUp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> FlowerCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;


};

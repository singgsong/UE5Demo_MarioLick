// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "Block_P.generated.h"

class ABlockBase;
class ALevelEndFlag;

/**
 * 
 */
UCLASS()
class MARIO_API ABlock_P : public ABaseActor
{
	GENERATED_BODY()
public:
	ABlock_P();
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ChangeBackToBlock();

	UPROPERTY(EditAnywhere)
	float InnerRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	float OuterRadius = 2000.f;

	UPROPERTY(EditAnywhere)
	float PBlockTime = 5.f;

	TArray<AActor*> ActorWithTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_RewardTiming;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> VFX_Hit_PBlock;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> HalfBlock;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ALevelEndFlag> LevelEnd;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALevelEndFlag> LevelEndClass;

};

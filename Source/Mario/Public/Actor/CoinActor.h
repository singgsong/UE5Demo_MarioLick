// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "CoinActor.generated.h"

class USphereComponent;

/**
 * 
 */
UCLASS()
class MARIO_API ACoinActor : public ABaseActor
{
	GENERATED_BODY()
public:	
	ACoinActor();

	void PlayCoinAnim(FTransform CoinInstanceTransform);
	USphereComponent* GetSphereCollision() { return SphereCollision; }
	void SetCanPickUp(bool Value) { bCanPickUp = Value; }
protected:
	virtual void BeginPlay() override;

private:
	void CoinAnim();

	UFUNCTION()
	void CoinSpinAndJump();

	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void PickUpCoin();

	UPROPERTY(EditAnywhere)
	bool bCanPickUp = true;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> PickUpCoinCurve;

	FOnTimelineEvent TimelintTickEvent;
	FOnTimelineEventStatic TimelineFinishDelegate;

	UPROPERTY(EditAnywhere)
	int32 Coins = 1;

	UPROPERTY(EditAnywhere)
	float CoinRewardAnimRate = 1.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> CoinMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> LocationZCurve;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> RotatorZCurve;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> VFX_CoinReward;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SFX_Coin;

};

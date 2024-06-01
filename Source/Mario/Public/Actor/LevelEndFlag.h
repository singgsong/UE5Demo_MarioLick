// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "LevelEndFlag.generated.h"

class UArrowComponent;
class APlayerCharacter;

DECLARE_MULTICAST_DELEGATE(FOnPlayerReachedDestination)

/**
 * 
 */
UCLASS()
class MARIO_API ALevelEndFlag : public ABaseActor
{
	GENERATED_BODY()
public:
	ALevelEndFlag();

	FOnPlayerReachedDestination OnPlayerReachedDestination;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void FlagPoleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayerAndFlagMoveToFlagPoleBottom();

private:
	UFUNCTION()
	void PlayerAndFlagDownFunc();
	void ReachedBottomFunc();

	UFUNCTION()
	void PlayerMoveToExplotionPoint();
	void PlayerExplotion();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Flag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> FlagPole;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> PoleTop;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> PlayerExplotionArrow;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> PoleCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> Fireworks;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_FlagPole;

	TObjectPtr<APlayerCharacter> Player;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> InterpCurve;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> VFX_PlayerExplotion;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Explotion;


	bool bRunOnce = false;

};

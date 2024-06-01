// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "BlockBase.generated.h"

class AMarioController;
class UInstancedStaticMeshComponent;
class UParticleSystem;
class APointsReward;
class APlayerCharacter;
class ACoinActor;
class ARedFlowerActor;
class USphereComponent;

UCLASS()
class MARIO_API ABlockBase : public ABaseActor
{
	GENERATED_BODY()

public:	
	ABlockBase();

	void PBlockEnable(bool bEnablebEnable);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void MeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AMarioController> MarioController;

	void HitBlockAnim();
	void PlayHitBlockAnim();

	void BreakBlock();

	void PlayPointsAnim();

	void CoinBlock(APlayerCharacter* Player);
	void PlayCoinAnim();

	void SpecialRewarBlock(APlayerCharacter* Player);

	void BecomeFakeBlock();

	UFUNCTION()
	void PBlockCoinCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	#pragma region HitBlock
	UPROPERTY(EditAnywhere, Category = "BlockType")
	bool bBreakable = false;

	UPROPERTY(EditAnywhere, Category = "BlockType")
	bool bVisible = true;

	UPROPERTY(EditAnywhere, Category = "BlockType")
	bool bSpecialReward = false;

	UPROPERTY(EditAnywhere, Category = "BlockType")
	bool bFlower = false;

	UPROPERTY(EditAnywhere, Category = "HitBlock")
	int32 HitTimes = 1;

	UPROPERTY(EditAnywhere, Category = "Reward|Coin")
	FTransform CoinInstanceTransform = FTransform(FRotator(90.f, 0.f, 0.f));

	UPROPERTY(EditDefaultsOnly, Category = "HitBlock")
	TObjectPtr<USoundBase> SFX_HitBlock;

	UPROPERTY(EditDefaultsOnly, Category = "HitBlock")
	TObjectPtr<UCurveFloat> HitBlockCurve;

	UPROPERTY(EditDefaultsOnly, Category = "HitBlock")
	TObjectPtr<UMaterialInterface> FakeBlockMaterial;
	#pragma endregion

	#pragma region BreakBlock
	UPROPERTY(EditDefaultsOnly, Category = "CanBreak")
	TObjectPtr<UParticleSystem> VFX_BreakBlock;

	UPROPERTY(EditDefaultsOnly, Category = "CanBreak")
	TObjectPtr<USoundBase> SFX_BreakBlock;
	#pragma endregion

	#pragma region Reward
	UPROPERTY(EditAnywhere, Category = "Reward")
	int32 BlockPoints = 10.f;

	FTimerHandle SpecialRewardTimer;
	bool TimerRunning = false;

	UPROPERTY(EditAnywhere, Category = "Reward")
	float SpecialRewardTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	TSubclassOf<APointsReward> PointsReward;

	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	TSubclassOf<ACoinActor> Coin;

	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	TSubclassOf<ARedFlowerActor> RedFlower;

	UPROPERTY(EditDefaultsOnly, Category = "Reward")
	TObjectPtr<USoundBase> SFX_FlowerGoOut;
	#pragma endregion

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> PBlockCoinCollision;

};

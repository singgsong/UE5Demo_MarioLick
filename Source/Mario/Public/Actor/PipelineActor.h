// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "PipelineActor.generated.h"

class UCapsuleComponent;

/**
 * 
 */
UCLASS()
class MARIO_API APipelineActor : public ABaseActor
{
	GENERATED_BODY()
public:
	APipelineActor();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void CapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void CapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void EnterUpPipe();

	UFUNCTION()
	void EnterDownPipe();

	UFUNCTION()
	void EnterDownPipeAnim();

	UFUNCTION()
	void ExitDownPipeAnim();

	UFUNCTION()
	void EnterUpPipeAnim();

	UFUNCTION()
	void ExitUpPipeAnim();
private:
	UPROPERTY(EditAnywhere)
	int32 PipelineLength = 1;

	UPROPERTY(EditAnywhere)
	bool bPipelineOnGround = true;

	UPROPERTY(EditAnywhere)
	bool bExitWayIsRise = true;

	UPROPERTY(EditAnywhere)
	bool bCanEnter = false;

	UPROPERTY(EditAnywhere)
	FVector Exit;

	UPROPERTY(EditAnywhere)
	bool bReadyEnter = false;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Pipeline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> EnterPipelineCurve;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> ExitPipelineCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> PipelineBodyMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> PipelineInsidMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> EnterUpPipelineAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> EnterDownPipelineAction;

	TObjectPtr<APlayerCharacter> Player;

};

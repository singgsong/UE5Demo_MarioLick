// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "PassablePatform.generated.h"

class UInputAction;
class USceneComponent;

UCLASS()
class MARIO_API APassablePatform : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	APassablePatform();

protected:
	virtual void BeginPlay() override;

private:	
	#pragma region ThroughPlatform
	UFUNCTION()
	virtual void BottomBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void BottomBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void TopBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void TopBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ThroughDown();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BottomBox;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> TopBox;

	bool bThroughPlatform = false;
	#pragma endregion

	#pragma region CanMove
	UFUNCTION()
	void PlatformMovement(float Alpha);

	UPROPERTY(EditAnywhere)
	float MoveRate = 1.f;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> MoveCurve;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> FirstPosition;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SecondPosition;

	FVector FirstPos;
	FVector SecondPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ThroughPlatformFromTopAction;

	UPROPERTY(EditAnywhere)
	bool bCanMove = false;
	#pragma endregion
};

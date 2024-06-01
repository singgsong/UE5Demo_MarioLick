// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "Mushroom.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class MARIO_API AMushroom : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	AMushroom();

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void BottomBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void BottomBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	virtual void HatCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void MushroomLaunchAnim(float Alpha);

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> HatScaleCurve;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BottomBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInstancedStaticMeshComponent> MushroomStem;

	UPROPERTY(EditAnywhere)
	FVector MushroomHatScale = FVector(3.f, 3.f, .3f);

	UPROPERTY(EditAnywhere)
	int32 StemCount = 2;

	UPROPERTY(EditAnywhere)
	FVector LaunchVelocity = FVector(0.f, 0.f, 1000.f);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> VFX_Launch;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_LaunchPlayer;
};

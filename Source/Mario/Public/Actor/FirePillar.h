// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseActor.h"
#include "FirePillar.generated.h"

class UInstancedStaticMeshComponent;
class URotatingMovementComponent;

/**
 * 
 */
UCLASS()
class MARIO_API AFirePillar : public ABaseActor
{
	GENERATED_BODY()
public:
	AFirePillar();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform);
	
	UFUNCTION()
	void PillarCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere)
	int32 PillarHeight = 2;

	UPROPERTY(EditAnywhere)
	int32 StaticMeshSize = 50;

	UPROPERTY(EditAnywhere)
	float PillarRotationRate = 50.f;

	UPROPERTY(EditAnywhere)
	float PillarBaseDamage = 1.f;

	UPROPERTY(EditAnywhere)
	float HitCooldown = 1.f;

	UPROPERTY(EditAnywhere)
	float KnocbackDistance = 500.f;

	UPROPERTY(EditAnywhere)
	float KnockupDistance = 500.f;

	UPROPERTY(EditAnywhere)
	FVector PlayerHitVolocity = FVector(0.f, 0.f, 500.f);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInstancedStaticMeshComponent> PillarMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URotatingMovementComponent> RotatingMovement;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> PillarCollision;


};

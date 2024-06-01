// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GoombaCharacter.generated.h"

class UBoxComponent;
class UPaperFlipbookComponent;
class AAIController;
class APointsReward;

UCLASS()
class MARIO_API AGoombaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGoombaCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void GoombaHitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GoombaKnockWall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GetHitCollisionBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	bool MoveLeft = false;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPaperFlipbookComponent> PaperFlipbook;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> GetHitCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> KnockWall;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAIController> GoombaAI;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> GoombaEnemy;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_GoombaDie;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APointsReward> PointsReward;

	UPROPERTY(EditAnywhere)
	float KnocbackDistance = 500.f;

	UPROPERTY(EditAnywhere)
	float KnockupDistance = 500.f;

};

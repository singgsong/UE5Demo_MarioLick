// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MARIO_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	void LoseHealth(int32 Value);
	void AddHealth(int32 Value);

	void AddCoins(int32 Value);
	void AddPoints(int32 Value);

private:	
	UPROPERTY(EditAnywhere)
	int32 MaxHealth = 3;

	UPROPERTY(EditAnywhere)
	int32 Health = 3;

	UPROPERTY(EditAnywhere)
	int32 Coins = 0;

	UPROPERTY(EditAnywhere)
	int32 Points = 0;

public:
	FORCEINLINE int32 GetMaxHealth() const { return MaxHealth; }

	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(int32 Value) { Health = Value; }

	FORCEINLINE int32 GetCoins() const { return Coins; }
	FORCEINLINE void SetCoins(int32 Value) { Coins = Value; }

	FORCEINLINE int32 GetPoints() const { return Points; }
	FORCEINLINE void SetPoints(int32 Value) { Points = Value; }

};

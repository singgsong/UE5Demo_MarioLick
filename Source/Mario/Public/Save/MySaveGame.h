// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameStruct.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MARIO_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FPlayerData PlayerData;

	FTransform ChackPointPositionData;

	int32 TimeLimitData;

public:
	FORCEINLINE FPlayerData GetPlayerSave() const { return PlayerData; }
	FORCEINLINE void SetPlayerData(const FPlayerData& Data) { PlayerData = Data; }

	FORCEINLINE FTransform GetChackPointPosition() const { return ChackPointPositionData; }
	FORCEINLINE void SetChackPointPosition(const FTransform& Pos) { ChackPointPositionData = Pos; }

	FORCEINLINE int32 GetTimeLimitData() const { return TimeLimitData; }
	FORCEINLINE void SetTimeLimitData(int32 Value) { TimeLimitData = Value; }

};

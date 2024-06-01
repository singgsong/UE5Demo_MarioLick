// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameStruct.h"
#include "SaveGameInterface.generated.h"

class UMySaveGame;
struct FPlayerData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MARIO_API ISaveGameInterface
{
	GENERATED_BODY()

public:
	virtual void SaveGame(bool Async) { }
	virtual void LoadGame(bool Async) { }

	virtual UMySaveGame* GetGameData() const = 0;
	virtual void SavePlayerData(const FPlayerData& PlayerData, bool Async) { }
	
};

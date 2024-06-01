// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveGameInterface.h"
#include "Character/PlayerCharacter.h"
#include "MyGameInstance.generated.h"

class UMySaveGame;
class USaveGame;
class UMainMenu;

/**
 * 
 */
UCLASS()
class MARIO_API UMyGameInstance : public UGameInstance, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void SaveGame(bool Async) override;
	virtual void LoadGame(bool Async) override;

	virtual void SavePlayerData(const FPlayerData& PlayerData, bool Async) override;

	virtual UMySaveGame* GetGameData() const override;

private:

	void LoadDataDelegateFunction(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMySaveGame> SaveData;

	FString SaveSlot = TEXT("Slot1");

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMySaveGame> SaveGameClass;


};

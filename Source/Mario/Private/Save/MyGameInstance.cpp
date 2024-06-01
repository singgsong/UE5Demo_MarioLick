#include "Save/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/MySaveGame.h"

void UMyGameInstance::Init()
{
	Super::Init();
	UGameplayStatics::DeleteGameInSlot(SaveSlot, 0);
	LoadGame(false);
}

void UMyGameInstance::SaveGame(bool Async)
{
	if (Async)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SaveData, SaveSlot, 0);
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(SaveData, SaveSlot, 0);
	}
}

void UMyGameInstance::LoadGame(bool Async)
{
	if (!SaveGameClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGameClass 未设置！"));
		return;
	}

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, 0))
	{
		if (Async)
		{
			FAsyncLoadGameFromSlotDelegate SlotDelegate;
			SlotDelegate.BindUObject(this, &ThisClass::LoadDataDelegateFunction);
			UGameplayStatics::AsyncLoadGameFromSlot(SaveSlot, 0, SlotDelegate);
		}
		else
		{
			SaveData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
		}
	}
	else
	{
		SaveData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
}

void UMyGameInstance::LoadDataDelegateFunction(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	SaveSlot = SlotName;
	SaveData = Cast<UMySaveGame>(LoadedGameData);
}

void UMyGameInstance::SavePlayerData(const FPlayerData& PlayerData, bool Async)
{
	SaveData->SetPlayerData(PlayerData);
	SaveGame(Async);
}

UMySaveGame* UMyGameInstance::GetGameData() const
{
	return SaveData;
}


#pragma once

#include "CoreMinimal.h"
#include "SaveGameStruct.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Health = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Coins = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Points = 0;
private:
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"
/**
 * 
 */
UCLASS()
class MARIO_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMainMenu> MainMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_MainMenuBGM;

};

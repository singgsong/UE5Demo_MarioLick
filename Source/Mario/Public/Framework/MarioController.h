// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MarioController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;
class ALevelEndFlag;
class UHUDWidget;

DECLARE_DELEGATE(FOnPlayerDied);
DECLARE_DELEGATE(FOnPlayerRespawn);

/**
 * 
 */
UCLASS()
class MARIO_API AMarioController : public APlayerController
{
	GENERATED_BODY()

public:
	AMarioController();


	void AddPoints(int32 Value);
	void AddCoins(int32 Value);
	void LoseHealth();
	int32 GetPlayHealth() const;

	void SaveAttribute(bool Param);
	void LoadAttribute();

	FOnPlayerDied OnPlayerDied;
	FOnPlayerRespawn OnPlayerRespawn;

protected:
	virtual void BeginPlay() override;

	void InitHUD();

	void StopBGMAndTimer();

	void PauseGame();

	void ContinueBGMAndTimer();

	void StartLevelTimer();
	void StartTimer();

	void PlayerReachDestination();

	void CalculateScore();
	void CalculateScoreAndSave();

	void NextLevel();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	TObjectPtr<UAttributeComponent> Attribute;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	TObjectPtr<UHUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> PauseGameAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_ClickButton;

	bool bShowMouse = false;

	bool HasChackPoint = false;

	TObjectPtr<APlayerCharacter> Player;

	#pragma region BGM
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> LevelMusic;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> WarningMusic;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> LevelCompleteMusic;
	#pragma endregion

	UPROPERTY(EditAnywhere)
	int32 TimeLimit = 60;

	UPROPERTY(EditAnywhere)
	int32 RemainingTime = 20;

	FTimerHandle LevelTimer;
	FTimerHandle CalculateTimer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALevelEndFlag> LevelEndClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Points;

public:
	FORCEINLINE bool GetHasChackPoint() const { return HasChackPoint; }
	FORCEINLINE void SetHasChackPoint(bool Value) { HasChackPoint = Value; }

	FORCEINLINE int32 GetTimeLimit() const { return TimeLimit; }
	FORCEINLINE void SetTimeLimit(int32 Value) { TimeLimit = Value; }

};

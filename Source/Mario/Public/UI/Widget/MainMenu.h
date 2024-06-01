// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class MARIO_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct() override;

	void ShowButton();

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void HoveredOnButton();

	UFUNCTION()
	void UnHoveredOnButton();
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowButtonAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ButtonAnim;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_ShowButton;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MarioHUD.generated.h"

class UHUDWidget;

/**
 * 
 */
UCLASS()
class MARIO_API AMarioHUD : public AHUD
{
	GENERATED_BODY()
public:
	UHUDWidget* GetHUDWidget() const { return HUDWidget; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHUDWidget> HUDWidget;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainMenuGameMode.h"
#include "UI/Widget/MainMenu.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (MainMenuClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		MainMenuWidget = CreateWidget<UMainMenu>(PlayerController, MainMenuClass);
		MainMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_MainMenuBGM);
	}

}

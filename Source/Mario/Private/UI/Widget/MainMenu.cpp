#include "UI/Widget/MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Save/MyGameInstance.h"

void UMainMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	FTimerHandle Timer;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UMainMenu::ShowButton);
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, 2.f, false);
}

void UMainMenu::NativeOnInitialized()
{
	StartButton->OnClicked.AddDynamic(this, &UMainMenu::StartGame);
	StartButton->OnHovered.AddDynamic(this, &UMainMenu::HoveredOnButton);
	StartButton->OnUnhovered.AddDynamic(this, &UMainMenu::UnHoveredOnButton);
}

void UMainMenu::ShowButton()
{
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_ShowButton);
	PlayAnimation(ShowButtonAnim);
}

void UMainMenu::StartGame()
{
	FTimerHandle Timer;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
		{
			//if (UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance()))
			//	GameInstance->LoadGame(false);
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level_1"));
		});
	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, .5f, false);
}

void UMainMenu::HoveredOnButton()
{
	PlayAnimation(ButtonAnim);
}

void UMainMenu::UnHoveredOnButton()
{
	PlayAnimation(ButtonAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
}


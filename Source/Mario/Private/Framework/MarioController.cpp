// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MarioController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "UI/Widget/MainMenu.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/LevelEndFlag.h"
#include "Framework/MarioGameMode.h"
#include "UI/HUD/MarioHUD.h"
#include "UI/Widget/HUDWidget.h"
#include "Character/Component/AttributeComponent.h"
#include "Interface/SaveGameInterface.h"
#include "Save/MySaveGame.h"

AMarioController::AMarioController()
{
	LevelMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("LevelMusci"));
	SetRootComponent(LevelMusic);

	WarningMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("WarningMusic"));
	WarningMusic->SetupAttachment(LevelMusic);
	WarningMusic->bAutoActivate = false;

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));

}

void AMarioController::BeginPlay()
{
	Super::BeginPlay();

	// 添加增强输入上下文
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(InputContext, 0);
	LoadAttribute();
	InitHUD();

	// 暂停游戏
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &AMarioController::PauseGame);

	// 关卡终点
	if (LevelEndClass)
	{
		ALevelEndFlag* LevelEnd = Cast<ALevelEndFlag>(UGameplayStatics::GetActorOfClass(GetWorld(), LevelEndClass));
		LevelEnd->OnPlayerReachedDestination.AddUObject(this, &ThisClass::PlayerReachDestination);
	}
	
	Player = Cast<APlayerCharacter>(GetPawn());

	StartLevelTimer();
	
	// 玩家死亡事件
	OnPlayerDied.BindUObject(this, &ThisClass::StopBGMAndTimer);

	OnPlayerRespawn.BindUObject(this, &ThisClass::ContinueBGMAndTimer);
}

void AMarioController::InitHUD()
{
	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	HUDWidget->UpdateHealth(Attribute->GetHealth());
	HUDWidget->UpdatePoints(Attribute->GetPoints());
	HUDWidget->UpdateCoins(Attribute->GetCoins());
	HUDWidget->UpdateTime(TimeLimit);
}

void AMarioController::AddPoints(int32 Value)
{
	Attribute->AddPoints(Value);
	HUDWidget->UpdatePoints(Attribute->GetPoints());
}

void AMarioController::AddCoins(int32 Value)
{
	Attribute->AddCoins(Value);
	HUDWidget->UpdateCoins(Attribute->GetCoins());
}

void AMarioController::LoseHealth()
{
	Attribute->LoseHealth(1);
	HUDWidget->UpdateHealth(Attribute->GetHealth());
}

int32 AMarioController::GetPlayHealth() const
{
	return Attribute->GetHealth();
}

void AMarioController::SaveAttribute(bool Param)
{
	FPlayerData Data;
	Data.Health = Attribute->GetHealth();
	Data.Coins = Attribute->GetCoins();
	Data.Points = Attribute->GetPoints();
	if (ISaveGameInterface* InterfaceIns = Cast<ISaveGameInterface>(GetWorld()->GetGameInstance()))
	{
		InterfaceIns->SavePlayerData(Data, Param);
	}
}

void AMarioController::LoadAttribute()
{
	if (ISaveGameInterface* InterfaceIns = Cast<ISaveGameInterface>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		FPlayerData Data = InterfaceIns->GetGameData()->GetPlayerSave();
		Attribute->SetHealth(Data.Health);
		Attribute->SetCoins(Data.Coins);
		Attribute->SetPoints(Data.Points);
	}
}

void AMarioController::StopBGMAndTimer()
{
	WarningMusic->Stop();
	LevelMusic->Stop();
	GetWorldTimerManager().PauseTimer(LevelTimer);
}

void AMarioController::ContinueBGMAndTimer()
{
	WarningMusic->Stop();
	LevelMusic->Play();
	GetWorldTimerManager().UnPauseTimer(LevelTimer);
}

void AMarioController::StartLevelTimer()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ThisClass::StartTimer);
	GetWorldTimerManager().SetTimer(LevelTimer, TimerDelegate, 1.f, true);
}

void AMarioController::StartTimer()
{
	if (TimeLimit--)
	{
		HUDWidget->UpdateTime(TimeLimit);

		// 时间即将结束警告
		if (RemainingTime == TimeLimit)
		{
			LevelMusic->SetPaused(true);
			WarningMusic->Play();

			FTimerHandle WarningTimer;
			FTimerDelegate WarningDelegate;
			WarningDelegate.BindLambda([this]()
				{
					LevelMusic->SetPaused(false);
					LevelMusic->SetPitchMultiplier(1.05);
				});
			GetWorldTimerManager().SetTimer(WarningTimer, WarningDelegate, 2.8f, false);
		}
	}
	else // 时间到了，杀死玩家
	{
		GetWorldTimerManager().ClearTimer(LevelTimer);
		LevelMusic->Stop();
		if (Player) Player->OnOverTime.ExecuteIfBound();
	}

}

void AMarioController::PlayerReachDestination()
{
	// 关闭音乐 
	LevelMusic->Stop();
	WarningMusic->Stop();

	// 停止关卡计时
	GetWorldTimerManager().ClearTimer(LevelTimer);

	// 3秒后计算总分
	FTimerHandle LevelCompleteTimer;
	FTimerDelegate LevelCompleteDelegate;
	LevelCompleteDelegate.BindUObject(this, &ThisClass::CalculateScore);
	GetWorldTimerManager().SetTimer(LevelCompleteTimer, LevelCompleteDelegate, 3.f, false);
}

void AMarioController::CalculateScore()
{
	FTimerDelegate CalculateDelegate;
	CalculateDelegate.BindUObject(this, &ThisClass::CalculateScoreAndSave);
	GetWorldTimerManager().SetTimer(CalculateTimer, CalculateDelegate, .05f, true);
}

void AMarioController::CalculateScoreAndSave()
{
	if (TimeLimit-- > 0)
	{
		if (Player)
		{
			HUDWidget->UpdateTime(TimeLimit);
			AddPoints(10);
		}
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_Points);

	}
	else
	{
		GetWorldTimerManager().ClearTimer(CalculateTimer);

		//GetWorldTimerManager().PauseTimer(CalculateTimer);
		SaveAttribute(true);

		FTimerHandle WaitToEnterNextLevelTimer;
		FTimerDelegate WaitToEnterNextLevel;
		WaitToEnterNextLevel.BindUObject(this, &ThisClass::NextLevel);
		GetWorldTimerManager().SetTimer(WaitToEnterNextLevelTimer, WaitToEnterNextLevel, 3.f, false);
	}
}

void AMarioController::NextLevel()
{
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Level_2"))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level_3"));
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Level_2"));
	}
}

void AMarioController::PauseGame()
{
	if (bShowMouse)
	{
		bShowMouse = true;
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_ClickButton);
	}
}



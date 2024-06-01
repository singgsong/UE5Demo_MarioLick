// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BlockBase.h"
#include "Components/StaticMeshComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/MarioController.h"
#include <Actor/PointsReward.h>
#include <Kismet/KismetMathLibrary.h>
#include "Actor/CoinActor.h"
#include "Actor/RedFlower/RedFlowerActor.h"
#include "Character/Component/AttributeComponent.h"
#include "Components/SphereComponent.h"

ABlockBase::ABlockBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PBlockCoinCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PBlockCoinCollision"));
	PBlockCoinCollision->SetupAttachment(Mesh);
}

void ABlockBase::BeginPlay()
{
	Super::BeginPlay();

	MarioController = Cast<AMarioController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!bVisible)
		Mesh->SetVisibility(false);

	if (bBreakable)
		Tags.Add(TEXT("PBlock"));

	HitBlockAnim();

	PBlockCoinCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Mesh->OnComponentHit.AddDynamic(this, &ThisClass::MeshComponentHit);
	PBlockCoinCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::PBlockCoinCollisionBeginOverlap);
}

void ABlockBase::PBlockEnable(bool bEnable)
{
	if (!Coin) return;
	if (!bEnable) // 变回方块
	{
		Mesh->SetVisibility(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), Coin, Actors);
		for (AActor* i : Actors)
		{
			i->Destroy();
		}
	}
	else // 方块变成金币
	{
		Mesh->SetVisibility(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ACoinActor* CA = GetWorld()->SpawnActorDeferred<ACoinActor>(Coin, GetActorTransform());
		CA->FinishSpawning(GetActorTransform());
		PBlockCoinCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void ABlockBase::PBlockCoinCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == OtherActor)
	{
		Destroy();
	}
}

void ABlockBase::MeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && Hit.ImpactNormal.Z == 1)
	{
		if (bBreakable) // 可击碎的方块
		{
			if (Player->GetHitPower() >= 2) // 力量足够以击碎方块，获得分数
			{
				BreakBlock();
				PlayPointsAnim();
				MarioController->AddPoints(BlockPoints);
			}
			else if (Player->GetHitPower() < 2) // 力量不足以击碎方块
			{
				UGameplayStatics::PlaySound2D(GetWorld(), SFX_HitBlock);
				PlayHitBlockAnim();
			}
		}
		else if(!bBreakable && HitTimes)// 不可击碎的方块：有金币方块、隐形方块和奖励方块
		{
			if (bSpecialReward) // 特殊奖励方块：1、限时金币奖励：第一次撞击后开始计时，期间可以撞击无数次。2、小花奖励
			{
				if (RedFlower) // 小花奖励
				{
					UGameplayStatics::PlaySound2D(GetWorld(), SFX_FlowerGoOut);
					ARedFlowerActor* Flower = GetWorld()->SpawnActor<ARedFlowerActor>(RedFlower, GetActorTransform());
					HitTimes = 0;
					BecomeFakeBlock();
				}
				else // 限时金币奖励
				{
					SpecialRewarBlock(Player);
					MarioController->AddPoints(BlockPoints);
					MarioController->AddCoins(1);
				}
			}
			else// 金币方块：被撞击一定次数后改变材质，每次撞击爆1枚金币
			{
				if (!bVisible) // 隐形方块
					Mesh->SetVisibility(true);

				CoinBlock(Player);
			}
		}
	}
}

void ABlockBase::BecomeFakeBlock()
{
	if (FakeBlockMaterial)
		Mesh->SetMaterial(0, FakeBlockMaterial);
}

void ABlockBase::CoinBlock(APlayerCharacter* Player)
{
	HitTimes--;
	PlayHitBlockAnim();
	PlayCoinAnim();
	MarioController->AddPoints(BlockPoints);
	MarioController->AddCoins(1);

	if (!HitTimes)
		BecomeFakeBlock();
}

void ABlockBase::PlayCoinAnim()
{
	if (!Coin) return;
	ACoinActor* Actor = GetWorld()->SpawnActorDeferred<ACoinActor>(Coin, GetActorTransform());
	Actor->PlayCoinAnim(FTransform(FRotator(0.f, 0.f, 90.f), FVector(0, 0, 0), Mesh->GetComponentScale()));
	Actor->SetCanPickUp(false);
	Actor->FinishSpawning(GetActorTransform());
}

void ABlockBase::SpecialRewarBlock(APlayerCharacter* Player)
{
	FTimerDelegate SpecialBlockDelegate;
	SpecialBlockDelegate.BindLambda([this]()
		{
			HitTimes = 0;
			TimerRunning = false;
			BecomeFakeBlock();
		});

	if (GetWorldTimerManager().IsTimerActive(SpecialRewardTimer))
	{
		PlayHitBlockAnim();
		PlayCoinAnim();
		//Player->AddPoints(BlockPoints);
	}
	else if (!TimerRunning) // 第一次撞奖励方块后开始计时！
	{
		TimerRunning = true;
		PlayHitBlockAnim();
		PlayCoinAnim();
		//Player->AddPoints(BlockPoints);
		GetWorldTimerManager().SetTimer(SpecialRewardTimer, SpecialBlockDelegate, SpecialRewardTime, false);
	}
}


void ABlockBase::HitBlockAnim()
{
	FOnTimelineFloatStatic BlockUpAndDown;
	BlockUpAndDown.BindLambda([this](float Z) { Mesh->SetRelativeLocation(FVector(0.f, 0.f, Z)); });
	if (HitBlockCurve)
		TimelineComponent->AddInterpFloat(HitBlockCurve, BlockUpAndDown);
}

void ABlockBase::PlayHitBlockAnim()
{
	TimelineComponent->PlayFromStart(); // Play() 不会自动重置TimelinePosition，即时间轴只执行一次
	TimelineComponent->GetTimelineFloatSignature();
}

void ABlockBase::PlayPointsAnim()
{
	if (PointsReward)
	{
		APointsReward* Actor = GetWorld()->SpawnActorDeferred<APointsReward>(PointsReward, GetActorTransform());
		Actor->SetPoints(BlockPoints);
		Actor->FinishSpawning(GetActorTransform());
	}
}

void ABlockBase::BreakBlock()
{
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_BreakBlock);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_BreakBlock, GetActorLocation());
	Destroy();
}




#include "Actor/CoinActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include <Character/PlayerCharacter.h>
#include "Components/StaticMeshComponent.h"
#include "Character/Component/AttributeComponent.h"

ACoinActor::ACoinActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(Mesh);
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CoinMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CoinMesh"));
	CoinMesh->SetupAttachment(ActorRootComponent);
	CoinMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

void ACoinActor::BeginPlay()
{
	Super::BeginPlay();

	if (bCanPickUp)
	{
		FOnTimelineEvent PickUpCoinEvent;
		PickUpCoinEvent.BindDynamic(this, &ThisClass::PickUpCoin);

		FOnTimelineFloat	PickUpCoinFloat;

		if (PickUpCoinCurve)
			TimelineComponent->AddInterpFloat(PickUpCoinCurve, PickUpCoinFloat);
		TimelineComponent->SetTimelinePostUpdateFunc(PickUpCoinEvent); // 使用多条轨道时调用该函数
		SetActorScale3D(FVector(.5f, .5f, .5f));
	}
	else
	{
		FOnTimelineFloat	RotatorZFloat;
		FOnTimelineFloat	LocationZFloat;

		TimelintTickEvent.BindDynamic(this, &ThisClass::CoinSpinAndJump);
		// 只要一条轨道时只需绑定LocationZFloat即可 (不完全对。。。）
		//LocationZFloat.BindDynamic(this, &ThisClass::FuckingFloatCurve); 

		if (LocationZCurve && RotatorZCurve)
		{
			TimelineComponent->AddInterpFloat(LocationZCurve, LocationZFloat);
			TimelineComponent->AddInterpFloat(RotatorZCurve, RotatorZFloat);
		}

		TimelineComponent->SetTimelinePostUpdateFunc(TimelintTickEvent); // 使用多条轨道时调用该函数
		TimelineComponent->SetPlayRate(CoinRewardAnimRate);

		// 方块生成的金币不应该有拾取碰撞箱
		SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
	}

	TimelineFinishDelegate.BindLambda([this]() { Destroy(); });

	TimelineComponent->SetTimelineFinishedFunc(TimelineFinishDelegate);

	//CoinAnim();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::SphereBeginOverlap);
}

void ACoinActor::PlayCoinAnim(FTransform CoinInstanceTransform)
{
	if (!VFX_CoinReward && !SFX_Coin) return;

	CoinMesh->AddInstance(CoinInstanceTransform);
	Mesh->SetVisibility(false);
	TimelineComponent->PlayFromStart();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_CoinReward, CoinMesh->GetComponentLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_Coin);
}

void ACoinActor::CoinSpinAndJump()
{
	if (!LocationZCurve && !RotatorZCurve) return;
	CoinMesh->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, LocationZCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition())), 
		FRotator(0.f, RotatorZCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition()), 0.f));
	//TimelineComponent->SetPlaybackPosition()
}

void ACoinActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_CoinReward, CoinMesh->GetComponentLocation());
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_Coin);
		TimelineComponent->PlayFromStart();
		MarioController->AddCoins(Coins);
		SphereCollision->DestroyComponent();
	}
}

void ACoinActor::PickUpCoin()
{
	Mesh->SetRelativeLocation(FVector(0, 0, PickUpCoinCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition())));
}


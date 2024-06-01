#include "Actor/Block_P.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/BlockBase.h"
#include "Actor/LevelEndFlag.h"

ABlock_P::ABlock_P()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Mesh);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	HalfBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HalfBlock"));
	HalfBlock->SetupAttachment(GetRootComponent());
}

void ABlock_P::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentHit.AddDynamic(this, &ThisClass::BoxHit);

	HalfBlock->SetVisibility(false);

	if (LevelEndClass)
	{
		LevelEnd = Cast<ALevelEndFlag>(UGameplayStatics::GetActorOfClass(GetWorld(), LevelEndClass));
		LevelEnd->OnPlayerReachedDestination.AddUObject(this, &ThisClass::ChangeBackToBlock);
	}
}

void ABlock_P::BoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter * Player = Cast<APlayerCharacter>(OtherActor);
	if (Player && Hit.ImpactNormal.Z)
	{
		// 踩中PBlock效果反馈
		Player->LaunchCharacter(FVector(0.f, 0.f, 1000.f), false, false);
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_RewardTiming);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_Hit_PBlock, GetActorLocation());
		if (CameraShake) UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, Player->GetActorLocation(), 1000.f, 2000.f);

		// 改变PBlock外观
		HalfBlock->SetVisibility(true);
		Mesh->DestroyComponent();
		BoxCollision->DestroyComponent();

		// 将所有带PBlock标签的方块，变成金币
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PBlock"), ActorWithTag);
		for (AActor* i : ActorWithTag)
		{
			ABlockBase* Block = Cast<ABlockBase>(i);
			Block->PBlockEnable(true);
		}

		// PBlock奖励时间结束，变回方块
		FTimerHandle Timer;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::ChangeBackToBlock);
		GetWorldTimerManager().SetTimer(Timer, TimerDelegate, PBlockTime, false);
	}
}

void ABlock_P::ChangeBackToBlock()	
{
	for (AActor* i : ActorWithTag)
	{
		ABlockBase* Block = Cast<ABlockBase>(i);
		Block->PBlockEnable(false);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/RedFlower/RedFlowerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/Component/AttributeComponent.h"

ARedFlowerActor::ARedFlowerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	FlowerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FlowerCollision"));
	FlowerCollision->SetupAttachment(Mesh);
	FlowerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ARedFlowerActor::BeginPlay()
{
	Super::BeginPlay();

	FlowerCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::FlowerCollisionBeginOverlap);

	if (Mesh)
	{
		FOnTimelineFloatStatic SpawnFlowerFloat;
		FOnTimelineEventStatic FlowerEffectEvent;

		SpawnFlowerFloat.BindLambda([this](float AxisZ) { Mesh->SetRelativeLocation(FVector(0.f, 0.f, AxisZ)); });

		FlowerEffectEvent.BindLambda([this]()
			{
				ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_Flower, GetActorLocation());
				FlowerCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
			});
		TimelineComponent->AddInterpFloat(SpawnFlowerCurve, SpawnFlowerFloat);
		TimelineComponent->SetTimelineFinishedFunc(FlowerEffectEvent);
		TimelineComponent->Play();
	}
}

void ARedFlowerActor::FlowerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (!Player->HasPowerUp())
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .2f);
			UGameplayStatics::PlaySound2D(GetWorld(), SFX_PlayerPowerUp);
			Player->PlayPowerUpAnim();
			MarioController->AddPoints(100.f);
			if (ParticleSystemComponent) ParticleSystemComponent->DestroyComponent();
			Destroy();
		}
	}
}

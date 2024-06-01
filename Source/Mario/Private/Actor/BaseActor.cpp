// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseActor.h"
#include "Kismet/GameplayStatics.h"

ABaseActor::ABaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ActorRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BlockRootComponent"));
	SetRootComponent(ActorRootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(ActorRootComponent);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("MovementTimeline"));
}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
	MarioController = Cast<AMarioController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PipelineActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <EnhancedInputComponent.h>
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

APipelineActor::APipelineActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PipelineInsidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipelineInsidMesh"));
	PipelineInsidMesh->SetupAttachment(GetRootComponent());

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(PipelineInsidMesh);

	PipelineBodyMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PipelineBodyMesh"));
	PipelineBodyMesh->SetupAttachment(PipelineInsidMesh);
}

void APipelineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (PipelineBodyMesh->PerInstanceSMData.Num() > PipelineLength) // 减少实例
		PipelineBodyMesh->PerInstanceSMData.Empty();

	for (int i = 0; i < PipelineLength; i++) // 增加实例
	{
		if (PipelineBodyMesh->PerInstanceSMData.Num() > i) continue;// 防止实例重叠 

		int32 Z = i * (-100);
		FVector temp = FVector(0.f, 0.f, Z);
		PipelineBodyMesh->AddInstance(FTransform(temp)); // 在 FVector(0.f, 0.f, i*(-100).f) 的位置生成StemCount个实例
	}
}

void APipelineActor::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(EnterUpPipelineAction, ETriggerEvent::Started, this, &ThisClass::EnterUpPipe);
		EnhancedInputComponent->BindAction(EnterDownPipelineAction, ETriggerEvent::Started, this, &ThisClass::EnterDownPipe);
	}

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::CapsuleBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::CapsuleEndOverlap);

	FOnTimelineFloatStatic EnterPipeFloatDelegate;
	FOnTimelineFloatStatic ExitPipeFloatDelegate;
	TimelineComponent->AddInterpFloat(EnterPipelineCurve, EnterPipeFloatDelegate);
	TimelineComponent->AddInterpFloat(ExitPipelineCurve, ExitPipeFloatDelegate);

}

void APipelineActor::CapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Player = Cast<APlayerCharacter>(OtherActor))
	{
		bReadyEnter = true;
	}
}

void APipelineActor::CapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Player = Cast<APlayerCharacter>(OtherActor))
	{
		bReadyEnter = false;
	}
}

void APipelineActor::EnterDownPipe()
{
	if (bCanEnter && bPipelineOnGround && bReadyEnter)
	{
		Player->DisableInput(GetWorld()->GetFirstPlayerController());
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_Pipeline);

		FOnTimelineEvent EnterPipeAnimEvent;
		EnterPipeAnimEvent.BindDynamic(this, &ThisClass::EnterDownPipeAnim);

		TimelineComponent->SetTimelinePostUpdateFunc(EnterPipeAnimEvent);
		TimelineComponent->PlayFromStart();
	}
}

void APipelineActor::EnterDownPipeAnim()
{
	bCanEnter = false;
	float CurveValue = EnterPipelineCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
	FVector Entrance = Mesh->GetComponentLocation();
	FVector Location = FVector(Entrance.X, Entrance.Y, Entrance.Z + 48.f + CurveValue);
	Player->GetCapsuleComponent()->SetRelativeLocationAndRotation(Location, FRotator(0, 0, 0));

	// EnterDownPipeAnim结束，播放ExitDownPipeAnim
	if (TimelineComponent->GetPlaybackPosition() == TimelineComponent->GetTimelineLength())
	{
		FOnTimelineEvent ExitPipeAnimEvent;
		ExitPipeAnimEvent.BindDynamic(this, &ThisClass::ExitDownPipeAnim);

		TimelineComponent->SetTimelinePostUpdateFunc(ExitPipeAnimEvent);
		TimelineComponent->PlayFromStart();
	}
}

void APipelineActor::ExitDownPipeAnim()
{
	bCanEnter = false;
	if (!bExitWayIsRise) // 退出管道的方式是掉下来
	{
		float CurveValue = EnterPipelineCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
		FVector Location = FVector(Exit.X, Exit.Y, Exit.Z + CurveValue);
		Player->GetCapsuleComponent()->SetRelativeLocationAndRotation(Location, FRotator(0, 0, 0));
	}
	else // 退出管道的方式是升起来
	{
		float CurveValue = ExitPipelineCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
		FVector Location = FVector(Exit.X, Exit.Y, Exit.Z - 96.f + CurveValue);
		Player->GetCapsuleComponent()->SetRelativeLocationAndRotation(Location, FRotator(0, 0, 0));

	}
	if (TimelineComponent->GetPlaybackPosition() == TimelineComponent->GetTimelineLength())
	{
		Player->EnableInput(GetWorld()->GetFirstPlayerController());
		bCanEnter = true;
	}
}

void APipelineActor::EnterUpPipe()
{
	if (bCanEnter && !bPipelineOnGround && bReadyEnter)
	{
		Player->DisableInput(GetWorld()->GetFirstPlayerController());
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_Pipeline);

		FOnTimelineEvent EnterPipeAnimEvent;
		EnterPipeAnimEvent.BindDynamic(this, &ThisClass::EnterUpPipeAnim);

		TimelineComponent->SetTimelinePostUpdateFunc(EnterPipeAnimEvent);
		TimelineComponent->PlayFromStart();
	}
}

void APipelineActor::EnterUpPipeAnim()
{
	bCanEnter = false;
	float CurveValue = ExitPipelineCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
	FVector Entrance = Mesh->GetComponentLocation();
	FVector Location = FVector(Entrance.X, Entrance.Y, Entrance.Z + 48.f + CurveValue);
	Player->GetCapsuleComponent()->SetRelativeLocationAndRotation(Location, FRotator(0, 0, 0));

	// EnterDownPipeAnim结束，播放ExitDownPipeAnim
	if (TimelineComponent->GetPlaybackPosition() == TimelineComponent->GetTimelineLength())
	{
		FOnTimelineEvent ExitPipeAnimEvent;
		ExitPipeAnimEvent.BindDynamic(this, &ThisClass::ExitUpPipeAnim);

		TimelineComponent->SetTimelinePostUpdateFunc(ExitPipeAnimEvent);
		TimelineComponent->PlayFromStart();
	}
}

void APipelineActor::ExitUpPipeAnim()
{
	bCanEnter = false;
	float CurveValue = ExitPipelineCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
	FVector Location = FVector(Exit.X, Exit.Y, Exit.Z - 96.f + CurveValue);
	Player->GetCapsuleComponent()->SetRelativeLocationAndRotation(Location, FRotator(0, 0, 0));

	if (TimelineComponent->GetPlaybackPosition() == TimelineComponent->GetTimelineLength())
	{
		Player->EnableInput(GetWorld()->GetFirstPlayerController());
		bCanEnter = true;
	}
}

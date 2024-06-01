// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PassablePatform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"

APassablePatform::APassablePatform()
{
	PrimaryActorTick.bCanEverTick = false;

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBox"));
	TopBox->SetupAttachment(Mesh);
	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	BottomBox->SetupAttachment(Mesh);

	FirstPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FirstPosition"));
	FirstPosition->SetupAttachment(Mesh);
	SecondPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SecondPosition"));
	SecondPosition->SetupAttachment(Mesh);
}

void APassablePatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(ThroughPlatformFromTopAction, ETriggerEvent::Started, this, &ThisClass::ThroughDown);
	}

	TopBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::TopBoxBeginOverlap);
	TopBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::TopBoxEndOverlap);
	BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BottomBoxBeginOverlap);
	BottomBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::BottomBoxEndOverlap);
	
	// PlatformMovement
	if (bCanMove)
	{
		FOnTimelineFloat	TimelineFloat;

		TimelineFloat.BindDynamic(this, &ThisClass::PlatformMovement);
		if (MoveCurve)
			TimelineComponent->AddInterpFloat(MoveCurve, TimelineFloat);
		FirstPos = FirstPosition->GetComponentLocation();
		SecondPos = SecondPosition->GetComponentLocation();
		TimelineComponent->SetLooping(true);
		TimelineComponent->SetPlayRate(MoveRate);
		TimelineComponent->Play();
	}
}

void APassablePatform::BottomBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APassablePatform::BottomBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void APassablePatform::TopBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{	
		bThroughPlatform = true;
	}
}

void APassablePatform::TopBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		bThroughPlatform = false;
	}
}

void APassablePatform::ThroughDown()
{
	if (bThroughPlatform)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APassablePatform::PlatformMovement(float Alpha)
{
	FVector MoveVector = UKismetMathLibrary::VLerp(FirstPos, SecondPos, Alpha);
	//Mesh->SetRelativeLocation(MoveVector); 相对父级位置设置Mesh的位置，因为这个Actor的根组件是Mesh所以这个的父级是世界场景的坐标，也就是FVector(0,0,0)
	SetActorLocation(MoveVector);
}


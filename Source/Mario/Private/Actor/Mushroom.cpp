// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Mushroom.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Character/PlayerCharacter.h>
#include "Kismet/GameplayStatics.h"

AMushroom::AMushroom()
{
	PrimaryActorTick.bCanEverTick = false;

	MushroomStem = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MushroomStem"));
	SetRootComponent(MushroomStem);

	Mesh->SetupAttachment(MushroomStem);

	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	BottomBox->SetupAttachment(Mesh);
}

// �ú����൱����ͼ��ConstructionScript
void AMushroom::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Mesh->SetWorldScale3D(MushroomHatScale);

	if (MushroomStem->PerInstanceSMData.Num() > StemCount) // ����ʵ��
	{
		MushroomStem->PerInstanceSMData.Empty();
	}
	for (int i = 0; i < StemCount; i++) // ����ʵ��
	{
		if (MushroomStem->PerInstanceSMData.Num() > i)// ��ֹʵ���ص�
		{
			continue;
		}
		int32 Z = i * (-100);
		FVector temp = FVector(0.f, 0.f, Z);
		MushroomStem->AddInstance(FTransform(temp)); // �� FVector(0.f, 0.f, i*(-100).f) ��λ������StemCount��ʵ��
	}
}

void AMushroom::BeginPlay()
{
	Super::BeginPlay();


	Mesh->OnComponentHit.AddDynamic(this, &ThisClass::HatCollisionHit);
	BottomBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BottomBoxBeginOverlap);
	BottomBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::BottomBoxEndOverlap);

	FOnTimelineFloat	TimelineFloat;

	TimelineFloat.BindDynamic(this, &ThisClass::MushroomLaunchAnim);
	if (HatScaleCurve)
		TimelineComponent->AddInterpFloat(HatScaleCurve, TimelineFloat);
}

void AMushroom::BottomBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AMushroom::BottomBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AMushroom::HatCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor);
	if (PC && Hit.ImpactNormal.Z == -1.f)
	{
		PC->LaunchCharacter(LaunchVelocity, false, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_Launch, Hit.ImpactPoint);
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_LaunchPlayer);
		TimelineComponent->PlayFromStart();
	}
}

void AMushroom::MushroomLaunchAnim(float Alpha)
{
	Mesh->SetRelativeScale3D(MushroomHatScale * Alpha);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FirePillar.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Component/AttributeComponent.h"

AFirePillar::AFirePillar()
{
	PrimaryActorTick.bCanEverTick = false;
	PillarMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PillarMesh"));
	PillarMesh->SetupAttachment(Mesh);
	PillarMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	PillarCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PillarCollision"));
	PillarCollision->SetupAttachment(PillarMesh);
	PillarCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PillarCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}

void AFirePillar::BeginPlay()
{
	Super::BeginPlay();
	RotatingMovement->RotationRate = FRotator(PillarRotationRate, 0.f, 0.f);
	PillarCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::PillarCollisionBeginOverlap);
}

void AFirePillar::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (PillarMesh->PerInstanceSMData.Num() > PillarHeight) // 减少实例
		PillarMesh->PerInstanceSMData.Empty();

	for (int32 i = 1; i <= PillarHeight; i++)
	{
		if (PillarMesh->PerInstanceSMData.Num() > i) // 防止实例重叠
			continue;

		int32 Z = i * (2 * StaticMeshSize);
		PillarMesh->AddInstance(FTransform(FVector(0.f, 0.f, Z)));

		/**
		* i * StaticMeshSize 是
		*/
		float LocationZ = Z / 2 + StaticMeshSize;
		float BoxExtentZ = i * StaticMeshSize;
		PillarCollision->SetRelativeLocation(FVector(0.f, 0.f, LocationZ));
		PillarCollision->SetBoxExtent(FVector(StaticMeshSize, StaticMeshSize, BoxExtentZ));
	}
}

void AFirePillar::PillarCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PillarCollisionBeginOverlap"));
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		FTimerHandle HitCooldownTimer;
		if (!GetWorldTimerManager().IsTimerActive(HitCooldownTimer))
		{
			Player->GetHIt(-1);
			Player->LaunchCharacter(FVector((KnocbackDistance * -Player->GetActorForwardVector().X), 0, KnockupDistance), true, true);
		}
		GetWorldTimerManager().SetTimer(HitCooldownTimer, HitCooldown, false);
	}
}

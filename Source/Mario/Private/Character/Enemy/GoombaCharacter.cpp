#include "Character/Enemy/GoombaCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actor/PointsReward.h"
#include "Framework/MarioController.h"

AGoombaCharacter::AGoombaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PaperFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbook"));
	PaperFlipbook->SetupAttachment(GetRootComponent());
	PaperFlipbook->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	GetHitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GetHitCollision"));
	GetHitCollision->SetupAttachment(PaperFlipbook);
	GetHitCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	KnockWall = CreateDefaultSubobject<UBoxComponent>(TEXT("KnockWall"));
	KnockWall->SetupAttachment(PaperFlipbook);
	KnockWall->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	KnockWall->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	KnockWall->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AGoombaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddMovementInput(GetActorForwardVector());
}

void AGoombaCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::GoombaHitPlayer);
	GetHitCollision->OnComponentHit.AddDynamic(this, &ThisClass::GetHitCollisionBegin);
	KnockWall->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::GoombaKnockWall);
}

void AGoombaCharacter::GoombaHitPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		FTimerHandle HitCooldownTimer;
		if (!GetWorldTimerManager().IsTimerActive(HitCooldownTimer))
		{
			Player->GetHIt(-1);
			Player->LaunchCharacter(FVector((KnocbackDistance * -Player->GetActorForwardVector().X), 0, KnockupDistance), true, true);
		}
		GetWorldTimerManager().SetTimer(HitCooldownTimer, 1.f, false);
	}
}

void AGoombaCharacter::GoombaKnockWall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!MoveLeft)
	{
		MoveLeft = true;
		SetActorRotation(FRotator(0.f, 180.f, 0.f));
	}
	else
	{
		MoveLeft = false;
		SetActorRotation(FRotator(0.f, 0.f, 0.f));
	}
}

// 古巴被踩死
void AGoombaCharacter::GetHitCollisionBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (Hit.ImpactNormal.Z == -1)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), SFX_GoombaDie);
			GetHitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			PaperFlipbook->SetWorldScale3D(FVector(0.25f, 0.25f, 0.1f));
			PaperFlipbook->AddWorldOffset(FVector(0.f, 0.f, -35.f));
			PaperFlipbook->Stop();
			Player->LaunchCharacter(FVector(0.f, 0.f, 300.f), true, true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// 得分动画
			if (PointsReward)
			{
				APointsReward* Actor = GetWorld()->SpawnActorDeferred<APointsReward>(PointsReward, GetActorTransform());
				Actor->SetPoints(50.f);
				if (AMarioController* MarioController = Cast<AMarioController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
				{
					MarioController->AddPoints(50.f);
				}
				Actor->FinishSpawning(GetActorTransform());
			}

			FTimerHandle DieTimer;
			FTimerDelegate DieDelegate;
			DieDelegate.BindLambda([this]() { Destroy(); });
			GetWorldTimerManager().SetTimer(DieTimer, DieDelegate, 3.f, false);

		}
	}
}

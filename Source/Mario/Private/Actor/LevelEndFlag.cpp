#include "Actor/LevelEndFlag.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

ALevelEndFlag::ALevelEndFlag()
{
	PrimaryActorTick.bCanEverTick = false;

	FlagPole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagPole"));
	FlagPole->SetupAttachment(Mesh);
	FlagPole->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Flag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag"));
	Flag->SetupAttachment(Mesh);
	Flag->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PoleTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoleTop"));
	PoleTop->SetupAttachment(Mesh);
	PoleTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PoleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PoleCollision"));
	PoleCollision->SetupAttachment(Mesh);
	PoleCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PoleCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	PlayerExplotionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerExplotionArrow"));
	PlayerExplotionArrow->SetupAttachment(GetRootComponent());

	Fireworks = CreateDefaultSubobject<UBoxComponent>(TEXT("Fireworks"));
	Fireworks->SetupAttachment(PlayerExplotionArrow);
	Fireworks->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Fireworks->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ALevelEndFlag::BeginPlay()
{
	Super::BeginPlay();

	bRunOnce = false;
	
	PoleCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::FlagPoleCollision);
}

void ALevelEndFlag::FlagPoleCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->GetCharacterMovement()->Deactivate();

		PlayerAndFlagMoveToFlagPoleBottom();
	}
}

void ALevelEndFlag::PlayerAndFlagMoveToFlagPoleBottom()
{
	// 从玩家从杆子上滑下来，旗帜下降
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_FlagPole);

	FOnTimelineFloat PlayerMoveEvent;
	FOnTimelineFloat FlagMoveEvent;
	FOnTimelineEvent PlayerAndFlagDown;
	FOnTimelineEventStatic ReachedBottomFunc;

	PlayerAndFlagDown.BindDynamic(this, &ThisClass::PlayerAndFlagDownFunc);
	ReachedBottomFunc.BindUObject(this, &ThisClass::ReachedBottomFunc);

	TimelineComponent->AddInterpFloat(InterpCurve, PlayerMoveEvent);
	TimelineComponent->AddInterpFloat(InterpCurve, FlagMoveEvent);
	TimelineComponent->SetTimelinePostUpdateFunc(PlayerAndFlagDown);
	TimelineComponent->SetTimelineFinishedFunc(ReachedBottomFunc);
	TimelineComponent->Play();
}

void ALevelEndFlag::PlayerAndFlagDownFunc()
{
	float Value = InterpCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
	float TargetLocaZ = Mesh->GetComponentLocation().Z + 100.f;
	float PlayerTargetLocaZ = UKismetMathLibrary::Lerp(Player->GetActorLocation().Z, TargetLocaZ, Value);
	float FlagTargetLocaZ = UKismetMathLibrary::Lerp(Flag->GetComponentLocation().Z, TargetLocaZ, Value);

	Player->SetActorLocation(FVector(Player->GetActorLocation().X, 0.f, PlayerTargetLocaZ));
	Flag->SetWorldLocation(FVector(Flag->GetComponentLocation().X, 0.f, FlagTargetLocaZ));
}

void ALevelEndFlag::ReachedBottomFunc()
{
	if (!bRunOnce)
	{
		bRunOnce = true;

		FOnTimelineFloat InterpFloat;
		FOnTimelineEvent PlayerExplotion;
		FOnTimelineEventStatic PlayerExplotionEvent;

		PlayerExplotion.BindDynamic(this, &ThisClass::PlayerMoveToExplotionPoint);
		PlayerExplotionEvent.BindUObject(this, &ThisClass::PlayerExplotion);

		TimelineComponent->AddInterpFloat(InterpCurve, InterpFloat);
		TimelineComponent->SetTimelinePostUpdateFunc(PlayerExplotion);
		TimelineComponent->SetTimelineFinishedFunc(PlayerExplotionEvent);
		TimelineComponent->PlayFromStart();
	}
}

void ALevelEndFlag::PlayerMoveToExplotionPoint()
{
	float Value = InterpCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());

	FVector Location = UKismetMathLibrary::VLerp(Player->GetActorLocation(), PlayerExplotionArrow->GetComponentLocation(), Value);
	Player->SetActorLocation(Location);
}

void ALevelEndFlag::PlayerExplotion()
{
	Player->GetCapsuleComponent()->SetVisibility(false, true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_PlayerExplotion, Player->GetActorLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_Explotion);

	OnPlayerReachedDestination.Broadcast();
}

#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Character/Component/AttributeComponent.h"
#include "Save/MyGameInstance.h"
#include "Save/MySaveGame.h"
#include "Framework/MarioGameMode.h"
#include "Components/ArrowComponent.h"
#include "Framework/MarioController.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

	AttachToFlagArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachToFlagArrow"));
	AttachToFlagArrow->SetupAttachment(GetRootComponent());

	SpringArm->bDoCollisionTest = false;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true; // 站在边缘不会悬空（即胶囊体不检测边缘）

	ControllerRotationSetting();
	MoveWay_2D();
	MovementSetting();
}

#pragma region MovementConfig
void APlayerCharacter::ControllerRotationSetting()
{
	SpringArm->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 1500.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = true;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::MovementSetting()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	// 落地不打滑
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->BrakingFriction = 1.f;

	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->JumpZVelocity = 1500.f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 2500.f, 0.f);
}

void APlayerCharacter::MoveWay_2D()
{
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
}
#pragma endregion

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	GameMode = Cast<AMarioGameMode>(GetWorld()->GetAuthGameMode());
	MarioController = Cast<AMarioController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	OnOverTime.BindUObject(this, &ThisClass::Die);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector ForwardVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, MoveValue.X);
	}
}

#pragma region GetRewardOrPower

void APlayerCharacter::PlayPowerUpAnim()
{
	bPowerUp = true;

	// 设置动画
	FOnTimelineFloatStatic PowerUpFloat;
	FOnTimelineEvent PowerUpEvent;
	FOnTimelineEventStatic ChangePowerEvent;
	PowerUpEvent.BindDynamic(this, &ThisClass::PowerUpFunc);
	ChangePowerEvent.BindLambda([this]() { UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f); }); // 重置时间膨胀

	TimelineComponent->AddInterpFloat(PowerUpCurve, PowerUpFloat);
	TimelineComponent->SetTimelinePostUpdateFunc(PowerUpEvent);
	TimelineComponent->SetTimelineFinishedFunc(ChangePowerEvent);
	TimelineComponent->SetIgnoreTimeDilation(true); // 时间轴不受时间膨胀影响

	// 播放动画
	GetCapsuleComponent()->SetCapsuleHalfHeight(120.f);
	//GetCapsuleComponent()->SetCapsuleRadius(40.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -120.f));
	TimelineComponent->PlayFromStart();
}

void APlayerCharacter::PowerUpFunc()
{
	float Value = PowerUpCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
	GetMesh()->SetRelativeScale3D(FVector(Value, Value, Value));
}

void APlayerCharacter::LoseFlowerPower()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	TimelineComponent->Reverse();
}
#pragma endregion

#pragma region Damage, DieAndRespawn
void APlayerCharacter::GetHIt(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("GetHIt"));

	if (!bPowerUp)
	{
		Die();
	}
	else
	{
		bPowerUp = false;
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_TakeDamage);
		LoseFlowerPower();
	}
}

void APlayerCharacter::Die()
{
	MarioController->OnPlayerDied.ExecuteIfBound();
	
	MarioController->LoseHealth();
	// 音效
	UGameplayStatics::PlaySound2D(GetWorld(), SFX_Die);

	// 动画
	GetCharacterMovement()->DisableMovement();
	DisableInput(GetWorld()->GetFirstPlayerController());
	GetMesh()->PlayAnimation(DieAnim, false);
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

	FOnTimelineFloatStatic Float;
	FOnTimelineEvent DieAnimEvent;
	DieAnimEvent.BindDynamic(this, &ThisClass::DieAnimFunc);
	
	TimelineComponent->AddInterpFloat(DieAnimCurve, Float);
	TimelineComponent->SetTimelinePostUpdateFunc(DieAnimEvent);
	TimelineComponent->PlayFromStart();

	// 重生
	if (MarioController->GetPlayHealth())
	{
		FTimerHandle RespawnTimer;
		FTimerDelegate RespawnTimerDelegate;
		RespawnTimerDelegate.BindUObject(this, &ThisClass::Respawn);
		GetWorldTimerManager().SetTimer(RespawnTimer, RespawnTimerDelegate, 2.8f, false);
	}
}

void APlayerCharacter::DieAnimFunc()
{
	float Value = DieAnimCurve->GetFloatValue(TimelineComponent->GetPlaybackPosition());
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + Value);
	GetMesh()->SetRelativeLocationAndRotation(Location, FRotator(0.f, 0.f, 0.f));
}

void APlayerCharacter::Respawn()
{
	AController* CortollerRef = GetController();
	
	MarioController->SaveAttribute(false);

	Destroy();


	if (bHasChackPoint) // 在记录点重生
	{
		if (ISaveGameInterface* Interface = Cast<ISaveGameInterface>(GameInstance))
		{
			if (Interface->GetGameData())
			{
				FTransform RespawnPos = Interface->GetGameData()->GetChackPointPosition();
				GameMode->RestartPlayerAtTransform(CortollerRef, RespawnPos);
			}
		}
	}
	else
	{
		GameMode->RestartPlayer(CortollerRef);
	}
	MarioController->LoadAttribute();

	if (AMarioController* PlayerController = Cast<AMarioController>(CortollerRef))
		PlayerController->OnPlayerRespawn.ExecuteIfBound();
}
#pragma endregion


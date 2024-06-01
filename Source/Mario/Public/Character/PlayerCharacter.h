#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SaveGameStruct.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UTimelineComponent;
class UAttributeComponent;
class UHUDWidget;
class UMyGameInstance;
class AMarioGameMode;
class AMarioController;

DECLARE_DELEGATE(FOnOverTime)

UCLASS()
class MARIO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayPowerUpAnim();

	void GetHIt(float DamageAmount);

	FOnOverTime OnOverTime;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void ControllerRotationSetting();
	void MoveWay_2D();
	void MovementSetting();


	UFUNCTION()
	void PowerUpFunc();

	void LoseFlowerPower();

	/*UFUNCTION()
	void Respawn(AActor* DestroyedActor);*/

	//void Respawn();

private: 
	#pragma region Locamotion
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Jump;
	#pragma endregion

	#pragma region HUD

	UPROPERTY(VisibleAnywhere, Category = "Sorse")
	int32 PointsAmount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Sorse")
	int32 CoinsAmount = 0;

	#pragma endregion

	#pragma region FlowerPower
	UPROPERTY(EditAnywhere, Category = "Power")
	int32 HitPower = 1;

	UPROPERTY(VisibleAnywhere, Category = "Power")
	bool bPowerUp = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Power")
	TObjectPtr<UCurveFloat> PowerUpCurve;
	#pragma endregion

	#pragma region GetHurt

	void Die();

	UFUNCTION()
	void DieAnimFunc();

	void Respawn();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_TakeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Die")
	TObjectPtr<USoundBase> SFX_Die;

	UPROPERTY(EditDefaultsOnly, Category = "Die")
	TObjectPtr<UAnimationAsset> DieAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Die")
	TObjectPtr<UCurveFloat> DieAnimCurve;

	bool bHasChackPoint = false;
	#pragma endregion

	TObjectPtr<UMyGameInstance> GameInstance;
	TObjectPtr<AMarioGameMode> GameMode;
	TObjectPtr<AMarioController> MarioController;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UArrowComponent> AttachToFlagArrow;

public:
	FORCEINLINE int32 GetHitPower() const { return HitPower; }

	FORCEINLINE bool HasPowerUp() const { return bPowerUp; }

	FORCEINLINE UMyGameInstance* GetGameInstance() const { return GameInstance; }
	FORCEINLINE AMarioGameMode* GetGameMode() const { return GameMode; }

	FORCEINLINE UArrowComponent* GetAttachToFlagArrow() { return AttachToFlagArrow; }

	FORCEINLINE void SetHasChackPoint(bool Value) { bHasChackPoint = Value; }

};

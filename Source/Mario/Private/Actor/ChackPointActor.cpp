#include "Actor/ChackPointActor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Save/MyGameInstance.h"
#include "Save/MySaveGame.h"
#include <Framework/MarioGameMode.h>

AChackPointActor::AChackPointActor()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Mesh);

	//RespawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RespawnArrow"));
	//RespawnArrow->SetupAttachment(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AChackPointActor::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BoxCollisionBeginOverlap);

}

void AChackPointActor::BoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->SetHasChackPoint(true);

		// 保存检查点位置
		Player->GetGameInstance()->GetGameData()->SetChackPointPosition(GetActorTransform()); 

		// 踩到检查的提示
		Mesh->SetMaterial(0, ActivatedMaterial);
		UGameplayStatics::PlaySound2D(GetWorld(), SFX_Activated);
	}
}

#include "Test/TestActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	FloatEvent1.BindUObject(this, &ATestActor::Float1Func);
	FloatEvent2.BindUObject(this, &ATestActor::Float2Func);
	UpdateFuncEvent.BindUFunction(this, TEXT("UpdateFunc"));
	FinishEvent.BindUObject(this, &ATestActor::FinishFunc);

	Timeline->AddInterpFloat(FloatCurve1, FloatEvent1);
	Timeline->AddInterpFloat(FloatCurve2, FloatEvent2);

	//Timeline->SetPlaybackPosition(2, false);
	//FloatCurve2->GetFloatValue(Timeline->GetPlaybackPosition());

	//Timeline->SetTimelinePostUpdateFunc(UpdateFuncEvent);
	Timeline->SetTimelineFinishedFunc(FinishEvent);

	Timeline->PlayFromStart();

}

void ATestActor::Float1Func(float Value)
{
	Mesh->AddWorldOffset(FVector(0.f, 0.f, Value));
}

void ATestActor::Float2Func(float Value)
{

	Mesh->AddWorldOffset(FVector(Value, 0.f, 0.f));
}

void ATestActor::FinishFunc()
{
	//FloatEvent1.Unbind();
	if (FloatEvent1.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsBound"));
	}
	Timeline->PlayFromStart();
}

void ATestActor::UpdateFunc()
{
	Mesh->AddWorldOffset(FVector(FloatCurve1->GetFloatValue(Timeline->GetPlaybackPosition()), 0.f, 0.f));


}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::LoseHealth(int32 Value)
{
	Health = FMath::Clamp(Health - Value, 0, MaxHealth);
}

void UAttributeComponent::AddHealth(int32 Value)
{
	Health = FMath::Clamp(Health + Value, 0, MaxHealth);
}

void UAttributeComponent::AddCoins(int32 Value)
{
	Coins = FMath::Clamp(Coins + Value, 0, 99999);
}

void UAttributeComponent::AddPoints(int32 Value)
{
	Points = FMath::Clamp(Points + Value, 0, 99999);
}


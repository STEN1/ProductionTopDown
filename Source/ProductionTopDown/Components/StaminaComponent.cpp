// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	Stamina = DefaultStamina;
	
}

int32 UStaminaComponent::GetStamina() const
{
	return Stamina;
}

int32 UStaminaComponent::GetDefaultStamina() const
{
	return DefaultStamina;
}
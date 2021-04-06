// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"

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
	GameModeRef = Cast<AProductionTopDownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
}

void UStaminaComponent::RegenerateStamina()
{
	Stamina += StaminaRegenerationAmount;
	if (Stamina > DefaultStamina)
	{
		Stamina = DefaultStamina;
		// stop timer..
		GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
	}
	GameModeRef->UpdateStaminaUI(Stamina, DefaultStamina);
}

float UStaminaComponent::GetStamina() const
{
	return Stamina;
}

float UStaminaComponent::GetDefaultStamina() const
{
	return DefaultStamina;
}

float UStaminaComponent::GetAttackCost() const
{
	return AttackCost;
}

float UStaminaComponent::GetDashCost() const
{
	return DashCost;
}

void UStaminaComponent::Attack()
{
	Stamina -= AttackCost;
	GameModeRef->UpdateStaminaUI(Stamina, DefaultStamina);
	// stop timer
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
	// start new timer
	GetWorld()->GetTimerManager().SetTimer(
		RegenerationTimerHandle,
		this, 
		&UStaminaComponent::RegenerateStamina, 
		StaminaRegenerationTimer, 
		true,
		StaminaRegenerationTimerStartDelay);
}

void UStaminaComponent::Dash()
{
	Stamina -= DashCost;
	GameModeRef->UpdateStaminaUI(Stamina, DefaultStamina);
	// stop timer
	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
	// start new timer
	GetWorld()->GetTimerManager().SetTimer(
		RegenerationTimerHandle,
		this,
		&UStaminaComponent::RegenerateStamina,
		StaminaRegenerationTimer,
		true,
		StaminaRegenerationTimerStartDelay);
}

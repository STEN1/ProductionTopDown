// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProductionTopDownGameModeBase.h"

#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"

void AProductionTopDownGameModeBase::ActorDied(AActor* DeadActor)
{
	if (DeadActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Died: %s"), *DeadActor->GetHumanReadableName());
	}
	
}

void AProductionTopDownGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Blueprint Event
	StartGame();
}

float AProductionTopDownGameModeBase::GetPlayerHealth() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UHealthComponent>()->GetHealth();
}

float AProductionTopDownGameModeBase::GetPlayerDefaultStamina() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UStaminaComponent>()->GetDefaultStamina();
}

float AProductionTopDownGameModeBase::GetPlayerStamina() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UStaminaComponent>()->GetStamina();
}

float AProductionTopDownGameModeBase::GetPlayerDefaultHealth() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UHealthComponent>()->GetDefaultHealth();
}

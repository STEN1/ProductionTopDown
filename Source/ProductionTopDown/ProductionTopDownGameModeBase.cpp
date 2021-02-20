// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProductionTopDownGameModeBase.h"

#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerPawn.h"

void AProductionTopDownGameModeBase::ActorDied(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor Died: %s"), *DeadActor->GetHumanReadableName());
}

void AProductionTopDownGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	// Blueprint Event
	StartGame();
}

int32 AProductionTopDownGameModeBase::GetPlayerHealth() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UHealthComponent>()->GetHealth();
}

int32 AProductionTopDownGameModeBase::GetPlayerDefaultStamina() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UHealthComponent>()->GetDefaultStamina();
}

int32 AProductionTopDownGameModeBase::GetPlayerStamina() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UHealthComponent>()->GetStamina();
}

int32 AProductionTopDownGameModeBase::GetPlayerDefaultHealth() const
{
	if (!Player) return 0;
	return Player->FindComponentByClass<UHealthComponent>()->GetDefaultHealth();
}

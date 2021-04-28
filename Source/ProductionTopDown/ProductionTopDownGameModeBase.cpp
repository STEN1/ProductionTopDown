// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProductionTopDownGameModeBase.h"

#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LevelStreaming.h"
#include "Engine/StaticMeshActor.h"

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

	// Enable overlap events for every StaticMeshActor so that spells can explode on overlap with env.
	// Might be a better way to do this? but this gives nice controll :)
	//GetWorldTimerManager().SetTimer(OverlapEventTimerHandle, [this]()
	//{
	//	
	//	TArray<AActor*> StaticMeshActors;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), StaticMeshActors);
	//	for (auto && StaticMeshActor : StaticMeshActors)
	//		StaticMeshActor->FindComponentByClass<UStaticMeshComponent>()->SetGenerateOverlapEvents(true);
	//}, 4.f, false);
	TArray<AActor*> StaticMeshActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), StaticMeshActors);
	for (auto && StaticMeshActor : StaticMeshActors)
		StaticMeshActor->FindComponentByClass<UStaticMeshComponent>()->SetGenerateOverlapEvents(true);
	
	// Blueprint Event

	StartGame();
}

void AProductionTopDownGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(OverlapEventTimerHandle);
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

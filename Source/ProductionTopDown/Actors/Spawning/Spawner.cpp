// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Spawnpoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawner::StartSpawning()
{
	if (DelayBetweenSpawn == 0.f)
	{
		InstantSpawn();
	}
	else if (DelayBetweenSpawn > 0.f)
	{
		SpawnArrayIndex = 0;
		bSpawningActors = true;
		SpawnWithTimer();
	}
}

void ASpawner::InstantSpawn()
{
	for (auto& SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint)
		{
			GetWorld()->SpawnActor<AActor>(
				SpawnPoint->ActorToSpawn,
				SpawnPoint->GetActorLocation(),
				SpawnPoint->GetActorRotation());	
		}
	}
}

void ASpawner::SpawnWithTimer()
{
	if (SpawnPoints.Num() <= 0 || SpawnPoints.Num() <= SpawnArrayIndex)
	{
		bSpawningActors = false;
		return;
	}

	if (SpawnPoints[SpawnArrayIndex])
	{
		GetWorld()->SpawnActor<AActor>(
            SpawnPoints[SpawnArrayIndex]->ActorToSpawn,
            SpawnPoints[SpawnArrayIndex]->GetActorLocation(),
            SpawnPoints[SpawnArrayIndex]->GetActorRotation());	
	}

	SpawnArrayIndex++;
	// Call this function again with a timer
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerTimerHandle, this, &ASpawner::SpawnWithTimer, DelayBetweenSpawn);
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	if (StartSpawningFromBeginPlay)
	{
		StartSpawning();
	}
	if (Trigger)
	{
		Trigger->OnActorBeginOverlap.AddDynamic(this, &ASpawner::BeginOverlapTrigger);
	}
}

void ASpawner::BeginOverlapTrigger(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetHumanReadableName());
	if (!ActorThatTriggers) return;
	if (!bSpawningActors && SpawnOnTriggerOverlap && OtherActor->IsA(ActorThatTriggers->ClassDefaultObject->GetClass()))
	{
		StartSpawning();
		SpawnOnTriggerOverlap = false;
		GetWorld()->GetTimerManager().SetTimer(TriggerCooldownTimerHandle, [this]()
		{
			SpawnOnTriggerOverlap = true;
		}, TriggerCooldown, false);
	}
}
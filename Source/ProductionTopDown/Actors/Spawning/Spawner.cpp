// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Spawnpoint.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ProductionTopDown/Actors/Interactables/ActivatableBase.h"
#include "ProductionTopDown/Character/EnemyBase.h"


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawner::StartSpawning()
{
	if (!bCanSpawn)
	{
		return;
	}
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
	AliveActors = SpawnPoints.Num();
	UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), AliveActors)
	if (bSpawnOnce)
	{
		bCanSpawn = false;
	}
}

void ASpawner::SpawnParticleEffect(FVector EffectSpawnLocationVector)
{
	EffectSpawnLocationVector.Z += 200.f;
	if (PSTemplate && !PSTemplate->IsLooping())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSTemplate, EffectSpawnLocationVector);
	}
	if (PSTemplate && PSTemplate->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping particle emitter from %s"), *GetHumanReadableName());
	}
	if (NSTemplate && !NSTemplate->IsLooping())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSTemplate, EffectSpawnLocationVector);
	}
	if (NSTemplate && NSTemplate->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping niagra particle emitter from %s"), *GetHumanReadableName());
	}
}

void ASpawner::OnClear()
{
	UE_LOG(LogTemp, Warning, TEXT("Room Clear! %s"), *GetHumanReadableName())
	ActivateActivatableArray();
	
	// play sound?
}

void ASpawner::ActivateActivatableArray()
{
	// Activatable is on a word :)
	for (auto& ActivatableActor : ActorsToActivateOnClear)
	{
		if (ActivatableActor)
		{
			ActivatableActor->Activate(true);
		}
	}
}

void ASpawner::ActorDied(AActor* DeadActor)
{
	AliveActors--;
	UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), AliveActors)
	if (AliveActors == 0)
	{
		OnClear();
	}
}

void ASpawner::InstantSpawn()
{
	for (auto& SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint)
		{
			AActor* TempActor = GetWorld()->SpawnActor<AActor>(
				SpawnPoint->ActorToSpawn,
				SpawnPoint->GetActorLocation(),
				SpawnPoint->GetActorRotation());
			SpawnParticleEffect(SpawnPoint->GetActorLocation());
			
			if (TempActor)
			{
				TempActor->SetOwner(this);
				OnActorSpawned(SpawnPoint->GetActorLocation());
				if(AEnemyBase* TempEnemy = Cast<AEnemyBase>(TempActor))
					TempEnemy->InitializeEnemyFromSpawner();
			}
			else
			{
				AliveActors--;
				UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), AliveActors)
			}
			
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
		AActor* TempActor = GetWorld()->SpawnActor<AActor>(
            SpawnPoints[SpawnArrayIndex]->ActorToSpawn,
            SpawnPoints[SpawnArrayIndex]->GetActorLocation(),
            SpawnPoints[SpawnArrayIndex]->GetActorRotation());
		SpawnParticleEffect(SpawnPoints[SpawnArrayIndex]->GetActorLocation());

		if (TempActor)
		{
			TempActor->SetOwner(this);
			OnActorSpawned(SpawnPoints[SpawnArrayIndex]->GetActorLocation());
			if(AEnemyBase* TempEnemy = Cast<AEnemyBase>(TempActor))
				TempEnemy->InitializeEnemyFromSpawner();
		}
		else
		{
			AliveActors--;
			UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), AliveActors)
		}
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
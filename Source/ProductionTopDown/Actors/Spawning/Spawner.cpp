// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Spawnpoint.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/BrushComponent.h"
#include "Components/CapsuleComponent.h"
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
		}
	}

	SpawnArrayIndex++;
	if (bLoopWhenOverlapped && SpawnPoints.Num() <= SpawnArrayIndex) SpawnArrayIndex = 0; 
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
		Trigger->GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASpawner::BeginOverlapTrigger);
		Trigger->GetBrushComponent()->OnComponentEndOverlap.AddDynamic(this, &ASpawner::EndOverlapTrigger);
	}
}

void ASpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(TriggerCooldownTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnTimerTimerHandle);
}

void ASpawner::BeginOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!ActorThatTriggers) return;
	if (!bSpawningActors && SpawnOnTriggerOverlap && OtherActor->IsA(ActorThatTriggers->ClassDefaultObject->GetClass()))
	{
		if (OtherActor->IsA(ACharacterBase::StaticClass()) && !OtherComp->IsA(UCapsuleComponent::StaticClass()))
		{
			return;
		}
		StartSpawning();
		SpawnOnTriggerOverlap = false;
		GetWorld()->GetTimerManager().SetTimer(TriggerCooldownTimerHandle, [this]()
		{
			SpawnOnTriggerOverlap = true;
		}, TriggerCooldown, false);
	}
}

void ASpawner::EndOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bLoopWhenOverlapped) return;
	if (OtherActor->IsA(ActorThatTriggers->ClassDefaultObject->GetClass()))
	{
		if (OtherActor->IsA(ACharacterBase::StaticClass()) && !OtherComp->IsA(UCapsuleComponent::StaticClass()))
		{
			return;
		}
		GetWorldTimerManager().ClearTimer(SpawnTimerTimerHandle);
		bSpawningActors = false;
	}
}

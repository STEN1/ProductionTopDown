// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();
	
	void StartSpawning();
	void InstantSpawn();
	void SpawnWithTimer();
	
protected:
	virtual void BeginPlay() override;
	
private:
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
    TArray<class ASpawnpoint*> SpawnPoints;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool StartSpawningFromBeginPlay{false};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool SpawnOnTriggerOverlap{false};
	// if this is zero the spawner will spawn all actors on the same frame.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float DelayBetweenSpawn{0.f};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float TriggerCooldown{2.f};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorThatTriggers;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	ATriggerVolume* Trigger;
	UFUNCTION()
	void BeginOverlapTrigger(AActor* OverlappedActor, AActor* OtherActor);

	
	
	bool bSpawningActors{false};
	int32 SpawnArrayIndex{0};
	FTimerHandle SpawnTimerTimerHandle;
	FTimerHandle TriggerCooldownTimerHandle;

};
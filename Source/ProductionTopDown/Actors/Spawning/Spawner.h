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

	void ActorDied(AActor* DeadActor);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings")
	class APatrolHub* PatrolHub{nullptr};

	bool bCanSpawn{true};
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintImplementableEvent)
    void OnActorSpawned(FVector SpawnLocationVector);
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool bSpawnOnce{true};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool bLoopWhenOverlapped{false};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TArray<class AActivatableBase*> ActorsToActivateOnClear;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
    TArray<class ASpawnpoint*> SpawnPoints;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool StartSpawningFromBeginPlay{false};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	bool SpawnOnTriggerOverlap{true};
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
	void BeginOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void EndOverlapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* PSTemplate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* NSTemplate;

	void SpawnParticleEffect(FVector EffectSpawnLocationVector);
	void OnClear();
	void ActivateActivatableArray();
	


	int32 AliveActors{0};
	bool bSpawningActors{false};
	int32 SpawnArrayIndex{0};
	FTimerHandle SpawnTimerTimerHandle;
	FTimerHandle TriggerCooldownTimerHandle;

};

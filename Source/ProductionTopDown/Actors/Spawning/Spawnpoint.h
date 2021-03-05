// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawnpoint.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API ASpawnpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnpoint();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<AActor> ActorToSpawn;

};

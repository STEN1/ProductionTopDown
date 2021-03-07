// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UMySaveGame();

	UPROPERTY(EditAnywhere)
	FVector PlayerLocation;
	UPROPERTY(EditAnywhere)
	FRotator PlayerRotation;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AItemBase>> Inventory;

	static void SaveGame(const UObject* WorldContextObject);
	static void LoadGame(const UObject* WorldContextObject);
};

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

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FRotator PlayerRotation;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<int32> ItemsDurability;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<TSubclassOf<class AItemBase>> Inventory;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 NumberOfHealthPots;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString Level;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SavePoint;

	UFUNCTION(BlueprintCallable)
	static bool SaveGame(const UObject* WorldContextObject, FString SlotName, FString SavePointName = "");
	UFUNCTION(BlueprintCallable)
	static void LoadGame(const UObject* WorldContextObject, FString SlotName);
};

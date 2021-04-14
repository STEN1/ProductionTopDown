// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 NumberOfHealthPots{0};
	UPROPERTY()
	TArray<int32> ItemsDurability{0, 0, 0, 0};
	UPROPERTY()
	TArray<class AItemBase*> Inventory;
	UPROPERTY()
	TArray<TSubclassOf<class AItemBase>> SavedInventory;
	UPROPERTY()
	FVector PosFromSaveGame{FVector::ZeroVector};
	UPROPERTY()
	FRotator RotFromSaveGame{FRotator::ZeroRotator};
	UPROPERTY()
	bool bLoadedGame{false};
	void SaveCurrentInventoryToInstance();
};

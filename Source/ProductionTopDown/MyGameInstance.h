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
	TArray<class AItemBase*> Inventory;
	UPROPERTY()
	TArray<TSubclassOf<class AItemBase>> SavedInventory;
	void SaveCurrentInventoryToInstance();
};

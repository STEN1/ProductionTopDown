// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "MyGameInstance.h"

void AMainMenuGameMode::ClearPlayerInventory()
{
	UMyGameInstance* GameInstance = GetGameInstance<UMyGameInstance>();
	GameInstance->SavedInventory.Empty();
	GameInstance->NumberOfHealthPots = 0;
}

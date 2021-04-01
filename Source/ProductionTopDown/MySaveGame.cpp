// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"



#include "MyGameInstance.h"
#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

UMySaveGame::UMySaveGame()
{
}

void UMySaveGame::SaveGame(const UObject* WorldContextObject, FString SlotName)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));

	if (Player)
	{
		SaveGameInstance->PlayerLocation = Player->GetActorLocation();
		SaveGameInstance->PlayerRotation = Player->GetActorRotation();

		SaveGameInstance->Level = WorldContextObject->GetWorld()->GetMapName();
		SaveGameInstance->Level.RemoveFromStart(WorldContextObject->GetWorld()->StreamingLevelsPrefix);
		UE_LOG(LogTemp, Warning, TEXT("Saved Level: %s"), *SaveGameInstance->Level);

		UInventoryComponent* InventoryComponent = Player->FindComponentByClass<UInventoryComponent>();
		
		if (InventoryComponent)
		{
			TArray<class AItemBase*> PlayerInventory = InventoryComponent->GetInventory();
			int32 InventorySize = InventoryComponent->GetInventorySize();
			SaveGameInstance->Inventory.Empty();
			SaveGameInstance->Inventory.SetNum(InventorySize);
			for (int32 i = 0; i < InventorySize; i++)
			{
				if (PlayerInventory[i])
				{
					SaveGameInstance->Inventory[i] = PlayerInventory[i]->GetClass();
				}
			}
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	UE_LOG(LogTemp, Warning, TEXT("Game Saved."));
}

void UMySaveGame::LoadGame(const UObject* WorldContextObject, FString SlotName)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	
	if (SaveGameInstance && GameInstance)
	{
		GameInstance->SavedInventory = SaveGameInstance->Inventory;
		GameInstance->PosFromSaveGame = SaveGameInstance->PlayerLocation;
		GameInstance->RotFromSaveGame = SaveGameInstance->PlayerRotation;
		GameInstance->bLoadedGame = true;

		UGameplayStatics::OpenLevel(WorldContextObject, *SaveGameInstance->Level);
		
		UE_LOG(LogTemp, Warning, TEXT("Game Loaded."));
	}
	
}

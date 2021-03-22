// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"


#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

UMySaveGame::UMySaveGame()
{
}

void UMySaveGame::SaveGame(const UObject* WorldContextObject)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));

	if (Player)
	{
		SaveGameInstance->PlayerLocation = Player->GetActorLocation();
		SaveGameInstance->PlayerRotation = Player->GetActorRotation();

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
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, WorldContextObject->GetWorld()->GetMapName(), 0);

	UE_LOG(LogTemp, Warning, TEXT("Game Saved."));
}

void UMySaveGame::LoadGame(const UObject* WorldContextObject)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(WorldContextObject->GetWorld()->GetMapName(), 0));
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));

	
	if (Player && SaveGameInstance)
	{
		Player->SetActorLocation(SaveGameInstance->PlayerLocation);
		Player->SetActorRotation(SaveGameInstance->PlayerRotation);
		
		UInventoryComponent* InventoryComponent = Player->FindComponentByClass<UInventoryComponent>();
		
		if (InventoryComponent)
		{
			InventoryComponent->LoadInventory(SaveGameInstance->Inventory);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Game Loaded."));
}

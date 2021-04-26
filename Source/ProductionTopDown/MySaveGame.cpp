// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"



#include "MyGameInstance.h"
#include "ToolBuilderUtil.h"
#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

UMySaveGame::UMySaveGame()
{
}

bool UMySaveGame::SaveGame(const UObject* WorldContextObject, FString SlotName, FString SavePointName)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));

	if (Player)
	{
		SaveGameInstance->PlayerLocation = Player->GetActorLocation();
		SaveGameInstance->PlayerRotation = Player->GetActorRotation();
		SaveGameInstance->SavePoint = SavePointName;

		SaveGameInstance->Level = WorldContextObject->GetWorld()->GetMapName();
		SaveGameInstance->Level.RemoveFromStart(WorldContextObject->GetWorld()->StreamingLevelsPrefix);

		UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (LoadGameInstance)
		{
			if (LoadGameInstance->SavePoint == SaveGameInstance->SavePoint
				&& LoadGameInstance->Level == SaveGameInstance->Level)
			{
				UE_LOG(LogTemp, Warning, TEXT("Already saved at point: %s"), *SaveGameInstance->SavePoint);
				return false;
			}
		}
		

		
		UE_LOG(LogTemp, Warning, TEXT("Saved Level: %s"), *SaveGameInstance->Level);

		UInventoryComponent* InventoryComponent = Player->FindComponentByClass<UInventoryComponent>();
		
		if (InventoryComponent)
		{
			SaveGameInstance->NumberOfHealthPots = InventoryComponent->GetNumberOfHealthPots();
			
			TArray<class AItemBase*> PlayerInventory = InventoryComponent->GetInventory();
			int32 InventorySize = InventoryComponent->GetInventorySize();
			SaveGameInstance->Inventory.Empty();
			SaveGameInstance->Inventory.SetNum(InventorySize);
			SaveGameInstance->ItemsDurability.Empty();
			SaveGameInstance->ItemsDurability.SetNum(InventorySize);
			for (int32 i = 0; i < InventorySize; i++)
			{
				if (PlayerInventory[i])
				{
					SaveGameInstance->Inventory[i] = PlayerInventory[i]->GetClass();
					SaveGameInstance->ItemsDurability[i] = PlayerInventory[i]->Durability;
				}
				else
				{
					SaveGameInstance->ItemsDurability[i] = 0;
				}
			}
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
	UE_LOG(LogTemp, Warning, TEXT("Slot Name: %s"), *SlotName);
	UE_LOG(LogTemp, Warning, TEXT("Game Saved."));
	return true;
}

void UMySaveGame::LoadGame(const UObject* WorldContextObject, FString SlotName)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	
	if (SaveGameInstance && GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Loaded. location %s"), *SaveGameInstance->PlayerLocation.ToCompactString());
		GameInstance->SavedInventory = SaveGameInstance->Inventory;
		GameInstance->PosFromSaveGame = SaveGameInstance->PlayerLocation;
		GameInstance->RotFromSaveGame = SaveGameInstance->PlayerRotation;
		GameInstance->NumberOfHealthPots = SaveGameInstance->NumberOfHealthPots;
		GameInstance->bLoadedGame = true;

		GameInstance->ItemsDurability.Empty();
		GameInstance->ItemsDurability.SetNum(SaveGameInstance->ItemsDurability.Num());
		for (int i = 0; i < SaveGameInstance->ItemsDurability.Num(); ++i)
		{
			GameInstance->ItemsDurability[i] = SaveGameInstance->ItemsDurability[i];
		}

		TArray<AActor*> ActorArray;
		UGameplayStatics::GetAllActorsOfClass(WorldContextObject, AActor::StaticClass(),ActorArray);
		for (auto Actor : ActorArray)
		{
			WorldContextObject->GetWorld()->GetTimerManager().ClearAllTimersForObject(Actor);
		}
		UGameplayStatics::OpenLevel(WorldContextObject, *SaveGameInstance->Level);
		
		
	}
	
}

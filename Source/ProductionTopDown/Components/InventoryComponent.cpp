// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"



#include "NiagaraFunctionLibrary.h"
#include "Chaos/AABBTree.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/MyGameInstance.h"
#include "ProductionTopDown/MySaveGame.h"
#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::DestroyWeapon()
{
	if (GameInstance->Inventory.Num() == 0)
	{
		return;
	}
	if (GameInstance->Inventory[CurrentSlot - 1])
	{
		if (!GameInstance->Inventory[CurrentSlot - 1]->IsWeapon())
		{
			return;
		}
		GameInstance->Inventory[CurrentSlot - 1]->Destroy();
		GameInstance->Inventory[CurrentSlot - 1] = nullptr;
		if (EmptySlotImage)
			GameModeRef->UpdateInventoryUI(CurrentSlot, EmptySlotImage);
		Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();

		SpawnParticleEffect(GetOwner()->GetActorLocation());

		if (WeaponBreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, WeaponBreakSound, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		}
	}
}

AItemBase* UInventoryComponent::GetItemObject() const
{
	if (GameInstance->Inventory[CurrentSlot - 1])
	{
		return GameInstance->Inventory[CurrentSlot - 1];
	}
	return nullptr;
}

TArray<AItemBase*> UInventoryComponent::GetInventory()
{
	return GameInstance->Inventory;
}

void UInventoryComponent::LoadInventory(TArray<TSubclassOf<class AItemBase>> LoadedInventory)
{
	InventorySize = LoadedInventory.Num();
	GameInstance->Inventory.Empty();
	GameInstance->Inventory.SetNum(InventorySize);
	for (int i = 0; i < InventorySize; ++i)
	{
		if (LoadedInventory[i] && GameInstance->Inventory[i])
		{
			GameInstance->Inventory[i]->Destroy();
			GameInstance->Inventory[i] = NewObject<AItemBase>(GetWorld(), LoadedInventory[i]);
		}
		else if (LoadedInventory[i])
		{
			GameInstance->Inventory[i] = NewObject<AItemBase>(GetWorld(), LoadedInventory[i]);
		}
		else
		{
			GameInstance->Inventory[i] = nullptr;
		}
		if (GameInstance->Inventory[i])
		{
			if (i == CurrentSlot - 1)
			{
				Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
			}
			GameModeRef->UpdateInventoryUI(i + 1, GameInstance->Inventory[i]->GetItemImage());
		}
		else if (EmptySlotImage)
		{
			GameModeRef->UpdateInventoryUI(i + 1, EmptySlotImage);
		}
	}
}

int32 UInventoryComponent::GetInventorySize() const
{
	return InventorySize;
}

int32 UInventoryComponent::GetNumberOfHealthPots() const
{
	return GameInstance->NumberOfHealthPots;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnActorBeginOverlap.AddDynamic(this, &UInventoryComponent::BeginOverlap);
	GetOwner()->OnActorEndOverlap.AddDynamic(this, &UInventoryComponent::EndOverlap);
	PlayerInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!PlayerInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No input component on: %s"), *GetOwner()->GetHumanReadableName());
	}
	else
	{
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &UInventoryComponent::Interact);
		PlayerInputComponent->BindAction("HealthPot", IE_Pressed, this, &UInventoryComponent::UseHealthPot);
		PlayerInputComponent->BindAction("InventorySlot1", IE_Pressed, this, &UInventoryComponent::Slot1);
		PlayerInputComponent->BindAction("InventorySlot2", IE_Pressed, this, &UInventoryComponent::Slot2);
		PlayerInputComponent->BindAction("InventorySlot3", IE_Pressed, this, &UInventoryComponent::Slot3);
		PlayerInputComponent->BindAction("InventorySlot4", IE_Pressed, this, &UInventoryComponent::Slot4);
		PlayerInputComponent->BindAction("Use", IE_Pressed, this, &UInventoryComponent::UseInventoryItem);
		PlayerInputComponent->BindAction("Save", IE_Pressed, this, &UInventoryComponent::Save);
		PlayerInputComponent->BindAction("Load", IE_Pressed, this, &UInventoryComponent::Load);
		PlayerInputComponent->BindAction("ThrowItem", IE_Pressed, this, &UInventoryComponent::ThrowItem);
		PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &UInventoryComponent::Pause);
	}
	GameModeRef = Cast<AProductionTopDownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeRef)
	{
		UE_LOG(LogTemp, Error, TEXT("No GameModeRef on: %s"), *GetOwner()->GetHumanReadableName());
	}
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Component: NO GAME INSTANCE REF!!!"))
	}
	// if there is a inventory in the game instance from a level change, or from loading from save.
	// load that inventory to the player inventory
	else if (GameInstance->SavedInventory.Num() == InventorySize)
	{
		GameInstance->Inventory.SetNum(InventorySize);
		for (int32 i = 0; i < InventorySize; ++i)
		{
			if (GameInstance->SavedInventory[i])
			{
				GameInstance->Inventory[i] = NewObject<AItemBase>(GetWorld(), GameInstance->SavedInventory[i]);
				GameModeRef->UpdateInventoryUI(i + 1, GameInstance->Inventory[i]->GetItemImage());
			}
			
		}
	}
	else if (IsInventoryEmpty()) // if the inventory is empty -> set the inventory to the correct size.
	{
		GameInstance->Inventory.SetNum(InventorySize);
		for (auto& Item : GameInstance->Inventory)
		{
			Item = nullptr;
		}
	}
	// if the level was loaded from a saved game-> set the players location and rotation to saved states.
	if (GameInstance)
	{
		if (GameInstance->bLoadedGame)
		{
			GetOwner()->SetActorLocation(GameInstance->PosFromSaveGame);
			GetOwner()->SetActorRotation(GameInstance->RotFromSaveGame);
			GameInstance->bLoadedGame = false;
		}
	}
	if (GameModeRef && GameInstance)
		GameModeRef->UpdateNumberOfHealthPots(GameInstance->NumberOfHealthPots);
	
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
}

void UInventoryComponent::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateOverlapArray();
}

void UInventoryComponent::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateOverlapArray();
}

void UInventoryComponent::UpdateOverlapArray()
{
	TArray<AActor*> TempArray;
	GetOwner()->GetOverlappingActors(TempArray, TSubclassOf<AItemBase>());
	OverlappingItems.Empty();
	OverlappingItems.SetNum(0);
	for (auto& TempActor : TempArray)
	{
		if (AItemBase* TempItem = Cast<AItemBase>(TempActor))
		{
			OverlappingItems.Push(TempItem);
		}
	}
}

void UInventoryComponent::Save()
{
	UMySaveGame::SaveGame(GetWorld(), "Slot1");
}

void UInventoryComponent::Load()
{
	UMySaveGame::LoadGame(GetWorld(), "Slot1");
}

void UInventoryComponent::Pause()
{
	GameModeRef->Pause();
}

void UInventoryComponent::SpawnParticleEffect(FVector EffectSpawnLocationVector)
{
	if (PSTemplate && !PSTemplate->IsLooping())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSTemplate, EffectSpawnLocationVector);
	}
	if (PSTemplate && PSTemplate->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping particle emitter from %s"), *GetOwner()->GetHumanReadableName());
	}
	if (NSTemplate && !NSTemplate->IsLooping())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSTemplate, EffectSpawnLocationVector);
	}
	if (NSTemplate && NSTemplate->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping niagra particle emitter from %s"), *GetOwner()->GetHumanReadableName());
	}
}

void UInventoryComponent::Interact()
{
	UpdateOverlapArray();
	if (OverlappingItems.Num() <= 0) return;

	if (PickUpHealthPot()) return;
	if (FillEmptySlot()) return;
	if (ReplaceCurrentSlot()) return;
}

void UInventoryComponent::UseHealthPot()
{
	if (GameInstance->NumberOfHealthPots > 0)
	{
		AHealthPickup::StaticClass()->GetDefaultObject<AHealthPickup>()->UseItem(
			Cast<APlayerCharacter>(GetOwner()), GetWorld()
		);
		GameInstance->NumberOfHealthPots--;
		GameModeRef->UpdateNumberOfHealthPots(GameInstance->NumberOfHealthPots);
	}

}

void UInventoryComponent::Slot1()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 1;
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
}

void UInventoryComponent::Slot2()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 2;
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
}

void UInventoryComponent::Slot3()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 3;
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
}

void UInventoryComponent::Slot4()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 4;
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
}

void UInventoryComponent::UseInventoryItem()
{
	if (GameInstance->Inventory[CurrentSlot - 1])
	{
		AItemBase* InventoryItem = GameInstance->Inventory[CurrentSlot - 1];
		InventoryItem->UseItem(Cast<APlayerCharacter>(GetOwner()), GetWorld());
		InventoryItem->OnUseItem(Cast<APlayerCharacter>(GetOwner()));
		if (InventoryItem->IsConsumable())
		{
			GameInstance->Inventory[CurrentSlot - 1]->Destroy();
			GameInstance->Inventory[CurrentSlot - 1] = nullptr;
			if (EmptySlotImage)
				GameModeRef->UpdateInventoryUI(CurrentSlot, EmptySlotImage);
			Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
		}
		
	}
}

void UInventoryComponent::ThrowItem()
{
	if (GameInstance->Inventory[CurrentSlot - 1])
	{
		GetWorld()->SpawnActor<AItemBase>(GameInstance->Inventory[CurrentSlot - 1]->GetClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		GameInstance->Inventory[CurrentSlot - 1]->Destroy();
		GameInstance->Inventory[CurrentSlot - 1] = nullptr;
		if (EmptySlotImage)
			GameModeRef->UpdateInventoryUI(CurrentSlot, EmptySlotImage);
		Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
	}
}

bool UInventoryComponent::PickUpHealthPot()
{
	if (OverlappingItems.Num() <= 0) return false;

	if (OverlappingItems.Last()->IsA(AHealthPickup::StaticClass()))
	{
		GameInstance->NumberOfHealthPots++;
		// update ui
		GameModeRef->UpdateNumberOfHealthPots(GameInstance->NumberOfHealthPots);
		
		OverlappingItems.Pop()->Destroy();
		return true;
	}
	
	return false;
}


bool UInventoryComponent::FillEmptySlot()
{
	if (OverlappingItems.Num() <= 0) return false;

	// See if any inventory slot is empty. Then fill that slot with object from overlapping items. 
	// Then destroy that item from the world.
	for (int32 i = 0;i < GameInstance->Inventory.Num();i++)
	{
		if (!GameInstance->Inventory[i])
		{
			GameInstance->Inventory[i] = NewObject<AItemBase>(GetWorld(), OverlappingItems.Last()->GetClass());
			// prevent GC
			// Inventory[i]->AddToRoot();
			OverlappingItems.Pop()->Destroy();
			GameModeRef->UpdateInventoryUI(i + 1, GameInstance->Inventory[i]->GetItemImage());
			Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::ReplaceCurrentSlot()
{
	if (OverlappingItems.Num() <= 0) return false;

	// check if there is already an item in the inventory. Then save that to a TempValue to spawn in the world.
	AItemBase* TempItem{ nullptr };
	if (GameInstance->Inventory[CurrentSlot - 1])
	{
		TempItem = GameInstance->Inventory[CurrentSlot - 1];
	}
	
	// Add item to inventory and Destroy it from the world. Then update ui if it is valid.
	GameInstance->Inventory[CurrentSlot - 1] = NewObject<AItemBase>(GetWorld(), OverlappingItems.Last()->GetClass());
	// prevent GC
	//Inventory[CurrentSlot - 1]->AddToRoot();
	OverlappingItems.Pop()->Destroy();
	if (GameInstance->Inventory[CurrentSlot - 1])
	{
		GameModeRef->UpdateInventoryUI(CurrentSlot, GameInstance->Inventory[CurrentSlot - 1]->GetItemImage());
	}

	// Now spawn item from inventory if there was an item there.
	if (TempItem)
	{
		GetWorld()->SpawnActor<AItemBase>(TempItem->GetClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
	
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
	
	return true;
}

bool UInventoryComponent::IsInventoryEmpty()
{
	for (auto Item : GameInstance->Inventory)
	{
		if (Item)
		{
			return false;
		}
	}
	return true;
}





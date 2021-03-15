// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"



#include "Chaos/AABBTree.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/MySaveGame.h"
#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Inventory.SetNum(InventorySize);
	for (auto& Item : Inventory)
	{
		Item = nullptr;
	}
}

AItemBase* UInventoryComponent::GetItemObject() const
{
	if (Inventory[CurrentSlot - 1])
	{
		return Inventory[CurrentSlot - 1].GetDefaultObject();
	}
	return nullptr;
}

TArray<TSubclassOf<AItemBase>> UInventoryComponent::GetInventory() const
{
	return Inventory;
}

void UInventoryComponent::LoadInventory(TArray<TSubclassOf<AItemBase>> LoadedInventory)
{
	InventorySize = LoadedInventory.Num();
	Inventory.Empty();
	Inventory.SetNum(InventorySize);
	for (int i = 0; i < InventorySize; ++i)
	{
		Inventory[i] = LoadedInventory[i];
		if (Inventory[i])
		{
			GameModeRef->UpdateInventoryUI(i + 1, Inventory[i].GetDefaultObject()->GetItemImage());
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
		PlayerInputComponent->BindAction("InventorySlot1", IE_Pressed, this, &UInventoryComponent::Slot1);
		PlayerInputComponent->BindAction("InventorySlot2", IE_Pressed, this, &UInventoryComponent::Slot2);
		PlayerInputComponent->BindAction("InventorySlot3", IE_Pressed, this, &UInventoryComponent::Slot3);
		PlayerInputComponent->BindAction("InventorySlot4", IE_Pressed, this, &UInventoryComponent::Slot4);
		PlayerInputComponent->BindAction("Use", IE_Pressed, this, &UInventoryComponent::UseInventoryItem);
		PlayerInputComponent->BindAction("Save", IE_Pressed, this, &UInventoryComponent::Save);
		PlayerInputComponent->BindAction("Load", IE_Pressed, this, &UInventoryComponent::Load);
		PlayerInputComponent->BindAction("ThrowItem", IE_Pressed, this, &UInventoryComponent::ThrowItem);
	}
	GameModeRef = Cast<AProductionTopDownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeRef)
	{
		UE_LOG(LogTemp, Error, TEXT("No GameModeRef on: %s"), *GetOwner()->GetHumanReadableName());
	}
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);

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
	UMySaveGame::SaveGame(GetWorld());
}

void UInventoryComponent::Load()
{
	UMySaveGame::LoadGame(GetWorld());
}

void UInventoryComponent::Interact()
{
	UpdateOverlapArray();
	UE_LOG(LogTemp, Warning, TEXT("OverlappingItems.Num(): %i"), OverlappingItems.Num())
	if (OverlappingItems.Num() <= 0) return;

	if (FillEmptySlot()) return;
	if (ReplaceCurrentSlot()) return;
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
	AItemBase* InventoryItem = Inventory[CurrentSlot - 1].GetDefaultObject();
	if (InventoryItem)
	{
		InventoryItem->UseItem(Cast<APlayerCharacter>(GetOwner()), GetWorld());
		InventoryItem->OnUseItem(Cast<APlayerCharacter>(GetOwner()));
		if (InventoryItem->IsConsumable())
		{
			Inventory[CurrentSlot - 1] = nullptr;
			if (EmptySlotImage)
				GameModeRef->UpdateInventoryUI(CurrentSlot, EmptySlotImage);
			Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
		}
		
	}
}

void UInventoryComponent::ThrowItem()
{
	if (Inventory[CurrentSlot - 1])
	{
		UE_LOG(LogTemp, Warning, TEXT("Throwing item: %s"), *Inventory[CurrentSlot - 1].GetDefaultObject()->GetName());
		GetWorld()->SpawnActor<AItemBase>(Inventory[CurrentSlot - 1], GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		Inventory[CurrentSlot - 1] = nullptr;
		if (EmptySlotImage)
			GameModeRef->UpdateInventoryUI(CurrentSlot, EmptySlotImage);
		Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
	}
}


bool UInventoryComponent::FillEmptySlot()
{
	if (OverlappingItems.Num() <= 0) return false;

	// See if any inventory slot is empty. Then fill that slot with object from overlapping items. 
	// Then destroy that item from the world.
	for (int32 i = 0;i < Inventory.Num();i++)
	{
		if (!Inventory[i])
		{
			Inventory[i] = OverlappingItems.Last()->GetClass();
			OverlappingItems.Pop()->Destroy();
			GameModeRef->UpdateInventoryUI(i + 1, Inventory[i].GetDefaultObject()->GetItemImage());
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::ReplaceCurrentSlot()
{
	if (OverlappingItems.Num() <= 0) return false;

	// check if there is already an item in the inventory. Then save that to a TempValue to spawn in the world.
	TSubclassOf<AItemBase> TempItem;
	if (Inventory[CurrentSlot - 1])
	{
		TempItem = Inventory[CurrentSlot - 1];
	}
	
	// Add item to inventory and Destroy it from the world. Then update ui if it is valid.
	Inventory[CurrentSlot - 1] = OverlappingItems.Last()->GetClass();
	OverlappingItems.Pop()->Destroy();
	if (Inventory[CurrentSlot - 1])
	{
		GameModeRef->UpdateInventoryUI(CurrentSlot, Inventory[CurrentSlot - 1].GetDefaultObject()->GetItemImage());
	}

	// Now spawn item from inventory if there was an item there.
	if (TempItem)
	{
		GetWorld()->SpawnActor<AItemBase>(TempItem, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
	
	Cast<APlayerCharacter>(GetOwner())->OnInventoryChange();
	
	return true;
}





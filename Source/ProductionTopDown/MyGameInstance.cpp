// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "Actors/Items/ItemBase.h"

void UMyGameInstance::SaveCurrentInventoryToInstance()
{
	SavedInventory.Empty();
	SavedInventory.SetNum(Inventory.Num());
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i])
		{
			SavedInventory[i] = Inventory[i]->GetClass();
		}
		else
		{
			SavedInventory[i] = nullptr;
		}
		
	}
}

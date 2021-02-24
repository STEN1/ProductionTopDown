// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"


#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "ProductionTopDown/Actors/Items/Weapons/WeaponBase.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Inventory.SetNum(4);
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
	}
	GameModeRef = Cast<AProductionTopDownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeRef)
	{
		UE_LOG(LogTemp, Error, TEXT("No GameModeRef on: %s"), *GetOwner()->GetHumanReadableName());
	}
	
}

void UInventoryComponent::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateOverlapArray();
	// update interact widget
}

void UInventoryComponent::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateOverlapArray();
	// update interact widget
}

void UInventoryComponent::UpdateOverlapArray()
{
	TArray<AActor*> TempArray;
	GetOwner()->GetOverlappingActors(TempArray, TSubclassOf<AItemBase>());
	OverlappingItems.Reset();
	for (auto& TempActor : TempArray)
	{
		if (AItemBase* TempItem = Cast<AItemBase>(TempActor))
		{
			OverlappingItems.Push(TempItem);
		}
	}
}

void UInventoryComponent::Interact()
{
	UpdateOverlapArray();
	if (OverlappingItems.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Interacting with Weapon: %s. Num(): %d"), *OverlappingWeapons[OverlappingWeapons.Num() - 1]->GetHumanReadableName(), OverlappingWeapons.Num());
		for (int32 i = 0;i < Inventory.Num();i++)
		{
			if (Inventory[i]._ItemClass == Empty)
			{
				Inventory[i]._ItemClass = OverlappingItems[OverlappingItems.Num() - 1]->GetItemClass();
				Inventory[i]._ItemName = OverlappingItems[OverlappingItems.Num() - 1]->GetItemName();
				Inventory[i]._ItemLore = OverlappingItems[OverlappingItems.Num() - 1]->GetItemLore();
				if (AWeaponBase* TempWeapon = Cast<AWeaponBase>(OverlappingItems[OverlappingItems.Num() - 1]))
				{
					Inventory[i]._MinDamage = TempWeapon->GetMinDamage();
					Inventory[i]._MaxDamage = TempWeapon->GetMaxDamage();
					Inventory[i]._CritChance = TempWeapon->GetCritChance();
				}
				break;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Inventory Num(): %i"), Inventory.Num());
		UE_LOG(LogTemp, Warning, TEXT("------------------------------"))
		for (auto& Weapon : Inventory)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Inventory: %s MinDMG: %f MaxDMG: %f CritChance %f"),
				*Weapon._ItemName,
				Weapon._MinDamage,
				Weapon._MaxDamage,
				Weapon._CritChance
				);
			UE_LOG(LogTemp, Warning, TEXT("Item Lore: %s"), *Weapon._ItemLore);
		}
		UE_LOG(LogTemp, Warning, TEXT("------------------------------"))
		OverlappingItems.Pop()->Destroy();
		
		
	}
}

void UInventoryComponent::Slot1()
{
	CurrentSlot = 1;
}

void UInventoryComponent::Slot2()
{
	CurrentSlot = 2;
}

void UInventoryComponent::Slot3()
{
	CurrentSlot = 3;
}

void UInventoryComponent::Slot4()
{
	CurrentSlot = 4;
}




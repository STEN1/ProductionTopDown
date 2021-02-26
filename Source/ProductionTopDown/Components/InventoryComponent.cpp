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
	GameModeRef->UpdateInventoryUICurrentSlot(CurrentSlot);
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
	if (OverlappingItems.Num() == 0) return;

	if (FillEmptySlot()) return;
	if (ReplaceCurrentSlot()) return;
}

void UInventoryComponent::Slot1()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 1;
	GameModeRef->UpdateInventoryUICurrentSlot(1);
}

void UInventoryComponent::Slot2()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 2;
	GameModeRef->UpdateInventoryUICurrentSlot(2);
}

void UInventoryComponent::Slot3()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 3;
	GameModeRef->UpdateInventoryUICurrentSlot(3);
}

void UInventoryComponent::Slot4()
{
	PreviousSlot = CurrentSlot;
	CurrentSlot = 4;
	GameModeRef->UpdateInventoryUICurrentSlot(4);
}

void UInventoryComponent::DropItem(ItemClass ItemToDrop)
{
	switch (ItemToDrop)
	{
		case Empty:
			UE_LOG(LogTemp, Warning, TEXT("Trying to drop from empty slot..."));
			break;
		case RustySword:
			GetWorld()->SpawnActor<ARustySword>(ItemRustySword, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("Spawning RustySword."));
			break;
		case HealthPickup:
			GetWorld()->SpawnActor<AHealthPickup>(ItemHealthPickup, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
			UE_LOG(LogTemp, Warning, TEXT("Spawning Health"));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Missing spawning implementation of item in Inventorycomponent.cpp -> DropItem() And probably missing TSubClassOf in BP"));
			break;
	}
}

bool UInventoryComponent::FillEmptySlot()
{
	for (int32 i = 0;i < Inventory.Num();i++)
	{
		if (Inventory[i]._ItemClass == Empty)
		{
			Inventory[i]._ItemClass = OverlappingItems[OverlappingItems.Num() - 1]->GetItemClass();
			Inventory[i]._ItemName = OverlappingItems[OverlappingItems.Num() - 1]->GetItemName();
			Inventory[i]._ItemLore = OverlappingItems[OverlappingItems.Num() - 1]->GetItemLore();
			Inventory[i]._ItemImage = OverlappingItems[OverlappingItems.Num() - 1]->GetItemImage();
			if (AWeaponBase* TempWeapon = Cast<AWeaponBase>(OverlappingItems[OverlappingItems.Num() - 1]))
			{
				Inventory[i]._MinDamage = TempWeapon->GetMinDamage();
				Inventory[i]._MaxDamage = TempWeapon->GetMaxDamage();
				Inventory[i]._CritChance = TempWeapon->GetCritChance();
			}
			GameModeRef->UpdateInventoryUI(i + 1, Inventory[i]._ItemImage);
			OverlappingItems.Pop()->Destroy();
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::ReplaceCurrentSlot()
{
	ItemClass ItemToDrop = Inventory[CurrentSlot - 1]._ItemClass;
	Inventory[CurrentSlot - 1]._ItemClass = OverlappingItems[OverlappingItems.Num() - 1]->GetItemClass();
	Inventory[CurrentSlot - 1]._ItemName = OverlappingItems[OverlappingItems.Num() - 1]->GetItemName();
	Inventory[CurrentSlot - 1]._ItemLore = OverlappingItems[OverlappingItems.Num() - 1]->GetItemLore();
	Inventory[CurrentSlot - 1]._ItemImage = OverlappingItems[OverlappingItems.Num() - 1]->GetItemImage();
	if (AWeaponBase* TempWeapon = Cast<AWeaponBase>(OverlappingItems[OverlappingItems.Num() - 1]))
	{
		Inventory[CurrentSlot - 1]._MinDamage = TempWeapon->GetMinDamage();
		Inventory[CurrentSlot - 1]._MaxDamage = TempWeapon->GetMaxDamage();
		Inventory[CurrentSlot - 1]._CritChance = TempWeapon->GetCritChance();
	}
	GameModeRef->UpdateInventoryUI(CurrentSlot, Inventory[CurrentSlot - 1]._ItemImage);
	OverlappingItems.Pop()->Destroy();
	DropItem(ItemToDrop);
	return true;
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"



#include "Chaos/AABBTree.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Inventory.SetNum(4);
	for (auto& Item : Inventory)
	{
		Item = nullptr;
	}
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
	UE_LOG(LogTemp, Warning, TEXT("Stepping on item"));
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
	OverlappingItems.Empty();
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
	UE_LOG(LogTemp, Warning, TEXT("OverlappingItems.Num(): %i"), OverlappingItems.Num())
	if (OverlappingItems.Num() <= 0) return;

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

void UInventoryComponent::DropItem()
{
	if (Inventory[CurrentSlot - 1])
	{
		GetWorld()->SpawnActor<AItemBase>(Inventory[CurrentSlot - 1]->GetClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant drop from empty slot"));
	}
	// switch (ItemToDrop)
	// {
	// 	case Empty:
	// 		UE_LOG(LogTemp, Warning, TEXT("Trying to drop from empty slot..."));
	// 		break;
	// 	case RustySword:
	// 		GetWorld()->SpawnActor<ARustySword>(ItemRustySword, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	// 		UE_LOG(LogTemp, Warning, TEXT("Spawning RustySword."));
	// 		break;
	// 	case HealthPickup:
	// 		GetWorld()->SpawnActor<AHealthPickup>(ItemHealthPickup, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	// 		UE_LOG(LogTemp, Warning, TEXT("Spawning Health"));
	// 		break;
	// 	default:
	// 		UE_LOG(LogTemp, Warning, TEXT("Missing spawning implementation of item in Inventorycomponent.cpp -> DropItem() And probably missing TSubClassOf in BP"));
	// 		break;
	// }
}

bool UInventoryComponent::FillEmptySlot()
{
	return false;
}

bool UInventoryComponent::ReplaceCurrentSlot()
{
	if (Inventory[CurrentSlot - 1])
	{
		Inventory[CurrentSlot - 1]->SetActorLocation(GetOwner()->GetActorLocation());
		Inventory[CurrentSlot - 1]->GetRootComponent()->SetVisibility(true, true);
		Inventory[CurrentSlot - 1]->FindComponentByClass<UBoxComponent>()->SetGenerateOverlapEvents(true);
	}
	
	OverlappingItems[OverlappingItems.Num() - 1]->GetRootComponent()->SetVisibility(false, true);
	OverlappingItems[OverlappingItems.Num() - 1]->FindComponentByClass<UBoxComponent>()->SetGenerateOverlapEvents(false);
	
	Inventory[CurrentSlot - 1] = OverlappingItems[OverlappingItems.Num() - 1];
	if (Inventory[CurrentSlot - 1] && Inventory[CurrentSlot - 1]->GetItemImage())
	{
		GameModeRef->UpdateInventoryUI(CurrentSlot, Inventory[CurrentSlot - 1]->GetItemImage());
	}
	return true;
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "ProductionTopDown/Actors/Items/Weapons/WeaponBase.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
	}
	
}

void UInventoryComponent::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateOverlapArrays();
	// update interact widget
}

void UInventoryComponent::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UpdateOverlapArrays();
	// update interact widget
}

void UInventoryComponent::UpdateOverlapArrays()
{
	TArray<AActor*> OverlappingActors;
	OverlappingItems.Reset();
	OverlappingWeapons.Reset();

	GetOwner()->GetOverlappingActors(OverlappingActors, TSubclassOf<AItemBase>());
	for (auto& Actor : OverlappingActors)
	{
		if (Cast<AWeaponBase>(Actor))
		{
			OverlappingWeapons.Push(Cast<AWeaponBase>(Actor));
		}
		else if (Cast<AItemBase>(Actor))
		{
			OverlappingItems.Push(Cast<AItemBase>(Actor));
		}
	}
}

void UInventoryComponent::Interact()
{
	UpdateOverlapArrays();
	if (OverlappingItems.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacting with Item: %s. Num(): %d"), *OverlappingItems[OverlappingItems.Num() - 1]->GetHumanReadableName(), OverlappingItems.Num());
		OverlappingItems.Pop()->Destroy();
	}
	else if (OverlappingWeapons.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacting with Weapon: %s. Num(): %d"), *OverlappingWeapons[OverlappingWeapons.Num() - 1]->GetHumanReadableName(), OverlappingWeapons.Num());
		OverlappingWeapons.Pop()->Destroy();
	}
}




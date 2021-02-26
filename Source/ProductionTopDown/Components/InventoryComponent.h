// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProductionTopDown/Actors/Items/HealthPickup.h"
#include "ProductionTopDown/Actors/Items/Weapons/RustySword.h"
#include "InventoryComponent.generated.h"


class AItemBase;
class AProductionTopDownGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRODUCTIONTOPDOWN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	void Interact();
	void Slot1();
	void Slot2();
	void Slot3();
	void Slot4();
	void DropItem();
	bool FillEmptySlot();
	bool ReplaceCurrentSlot();
	int32 PreviousSlot{ 1 };
	int32 CurrentSlot{ 1 };

	void UpdateOverlapArray();
	TArray<AItemBase*> OverlappingItems;
	
	TArray<AItemBase*> Inventory;

	UPROPERTY()
	UInputComponent* PlayerInputComponent;
	UPROPERTY()
	AProductionTopDownGameModeBase* GameModeRef;

	UPROPERTY(EditDefaultsOnly, Category = "Items to spawn on drop")
	TSubclassOf<ARustySword> ItemRustySword;
	UPROPERTY(EditDefaultsOnly, Category = "Items to spawn on drop")
	TSubclassOf<AHealthPickup> ItemHealthPickup;
};

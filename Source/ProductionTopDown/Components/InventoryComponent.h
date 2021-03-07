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
	
	AItemBase* GetItemObject() const;
	TArray<TSubclassOf<AItemBase>> GetInventory() const;
	void LoadInventory(TArray<TSubclassOf<AItemBase>> LoadedInventory);
	int32 GetInventorySize() const;
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
	void UseInventoryItem();
	bool FillEmptySlot();
	bool ReplaceCurrentSlot();
	int32 PreviousSlot{ 1 };
	int32 CurrentSlot{ 1 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EmptySlotImage", meta = (AllowPrivateAccess = "true"))
	UTexture2D* EmptySlotImage;

	void UpdateOverlapArray();
	TArray<AItemBase*> OverlappingItems;
	
	TArray<TSubclassOf<AItemBase>> Inventory;

	UPROPERTY()
	UInputComponent* PlayerInputComponent;
	UPROPERTY()
	AProductionTopDownGameModeBase* GameModeRef;

	int32 InventorySize{4};

	void Save();
	void Load();
};

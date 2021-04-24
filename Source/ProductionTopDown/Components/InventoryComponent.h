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

	void DestroyWeapon();
	AItemBase* GetItemObject() const;
	TArray<AItemBase*> GetInventory();
	void LoadInventory(TArray<TSubclassOf<class AItemBase>> LoadedInventory);
	int32 GetInventorySize() const;
	int32 GetNumberOfHealthPots() const;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	void Interact();
	void UseHealthPot();
	void PlayHealthPotUseEffects();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* PSHealthPotUseEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* NSHealthPotUseEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	USoundBase* HealthPotUseSound;
	void Slot1();
	void Slot2();
	void Slot3();
	void Slot4();
	void UseInventoryItem();
	void ThrowItem();
	bool PickUpHealthPot();
	bool FillEmptySlot();
	bool ReplaceCurrentSlot();
	bool IsInventoryEmpty();
	int32 PreviousSlot{ 1 };
	int32 CurrentSlot{ 1 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EmptySlotImage", meta = (AllowPrivateAccess = "true"))
	UTexture2D* EmptySlotImage;

	void UpdateOverlapArray();

	UPROPERTY()
	TArray<AItemBase*> OverlappingItems;

	UPROPERTY()
	UInputComponent* PlayerInputComponent;
	UPROPERTY()
	AProductionTopDownGameModeBase* GameModeRef;
	UPROPERTY()
	class UMyGameInstance* GameInstance;

	int32 InventorySize{4};

	void Save();
	void Load();

	void Pause();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	USoundBase* WeaponBreakSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* PSWeaponBreakEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* NSWeaponBreakEffect;
	void SpawnBreakParticleEffect(FVector EffectSpawnLocationVector);


};

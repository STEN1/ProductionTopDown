// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemBase.generated.h"
enum ItemClass
{
	Empty,
    RustySword,
	HealthPickup
};
UCLASS()
class PRODUCTIONTOPDOWN_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();
	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	FString GetItemName() const;
	UFUNCTION(BlueprintCallable)
	FString GetItemLore() const;
	UFUNCTION(BlueprintCallable)
	float GetMinDamage() const;
	UFUNCTION(BlueprintCallable)
    float GetMaxDamage() const;
	UFUNCTION(BlueprintCallable)
    float GetCritChance() const;
	float GetKnockbackAmount() const;
	
	ItemClass GetItemClass() const;
	UTexture2D* GetItemImage() const;
	UStaticMeshComponent* GetItemStaticMesh() const;
	virtual void UseItem(class APlayerCharacter* PlayerCharacter, class UWorld* World);
	UFUNCTION(BlueprintImplementableEvent)
	void OnUseItem(class APlayerCharacter* PlayerCharacter);
	bool IsConsumable();
	bool IsWeapon();
	
	static void SpawnItemOfItemClass(ItemClass Item, AActor* Actor, FVector Location, FRotator Rotation);

	
protected:
	virtual void BeginPlay() override;
	bool bUsable{ true };
	bool bPickup{ false };
	bool bConsumable{ false };

	ItemClass ThisItemClass{ Empty };

	FString ItemName{TEXT("")};
	FString ItemLore{TEXT("")};
	float MinDamage{8.f};
	float MaxDamage{13.f};
	float CritChance{30.f}; // Percentage

	float KnockbackAmount{200.f};

	bool bIsWeapon{false};
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	UTexture2D* ItemImage;
	
	//
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	
	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemBase.generated.h"

class UNiagaraSystem;

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

	UNiagaraSystem* GetLightAttackEffect() const;
	UNiagaraSystem* GetHeavyAttackEffect() const;
	
	ItemClass GetItemClass() const;
	UTexture2D* GetItemImage() const;
	UStaticMeshComponent* GetItemStaticMesh() const;
	virtual void UseItem(class APlayerCharacter* PlayerCharacter, class UWorld* World);
	UFUNCTION(BlueprintImplementableEvent)
	void OnUseItem(class APlayerCharacter* PlayerCharacter);
	bool IsConsumable();
	bool IsWeapon();
	
	static void SpawnItemOfItemClass(ItemClass Item, AActor* Actor, FVector Location, FRotator Rotation);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings")
	int32 Durability{50};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings")
	UParticleSystem* ItemParticleSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings")
	UNiagaraSystem* LightAttackEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings")
	UNiagaraSystem* HeavyAttackEffect;
	
protected:
	virtual void BeginPlay() override;
	bool bUsable{ true };
	bool bPickup{ false };
	bool bConsumable{ false };

	ItemClass ThisItemClass{ Empty };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	FString ItemName{TEXT("")};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	FString ItemLore{TEXT("")};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	float MinDamage{8.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	float MaxDamage{13.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	float CritChance{30.f}; // Percentage
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	float KnockbackAmount{200.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
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

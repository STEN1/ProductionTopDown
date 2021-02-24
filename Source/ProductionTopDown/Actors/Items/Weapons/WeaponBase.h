// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "WeaponBase.generated.h"

/**
 * 
 */

UCLASS()
class PRODUCTIONTOPDOWN_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
public:
	AWeaponBase();
	
	UFUNCTION(BlueprintCallable)
	float GetMinDamage() const;
	UFUNCTION(BlueprintCallable)
    float GetMaxDamage() const;
	UFUNCTION(BlueprintCallable)
    float GetCritChance() const;
	
	float GetKnockbackAmount() const;
	
protected:
	virtual void BeginPlay() override;
	
	float MinDamage{8.f};
	float MaxDamage{13.f};
	float CritChance{30.f}; // Percentage

	float KnockbackAmount{200.f};
	
};

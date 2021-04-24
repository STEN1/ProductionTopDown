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
	
	virtual bool UseItem(class APlayerCharacter* PlayerCharacter, UWorld* World) override;
	protected:
	virtual void BeginPlay() override;
	
	
};

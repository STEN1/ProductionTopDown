// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API AHealthPickup : public AItemBase
{
	GENERATED_BODY()
public:
	AHealthPickup();

	virtual void UseItem(class APlayerCharacter* PlayerCharacter, UWorld* World) override;
protected:
	virtual void BeginPlay() override;
	
};

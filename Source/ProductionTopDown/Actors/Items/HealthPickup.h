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
protected:
	virtual void BeginPlay() override;
	
};

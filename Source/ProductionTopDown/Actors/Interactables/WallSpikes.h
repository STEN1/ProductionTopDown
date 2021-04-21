// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProductionTopDown/Actors/Interactables/ActivatableBase.h"
#include "WallSpikes.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API AWallSpikes : public AActivatableBase
{
	GENERATED_BODY()

public:
	AWallSpikes();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	
};

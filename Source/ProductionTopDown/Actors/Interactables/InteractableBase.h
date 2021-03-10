// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

	virtual void Interact(bool Condition = true);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};

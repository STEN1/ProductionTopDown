// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "SpikeTrap.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API ASpikeTrap : public AInteractableBase
{
	GENERATED_BODY()

public:
	ASpikeTrap();
	
	void Tick(float DeltaSeconds) override;

	virtual void Interact(bool Condition = true);

	UPROPERTY(VisibleAnywhere, Category="Setup")
	UStaticMeshComponent* BaseMesh{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	UStaticMeshComponent* SpikeMesh{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	FVector Stage1Offset{10.f, 10.f, 10.f};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	FVector Stage2Offset{20.f, 20.f, 20.f};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	float Stage1Timer{1.f};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	float Stage2Timer{1.f};

	
	
protected:
	virtual void BeginPlay() override;
	
private:
	FVector StartLocation;
	FVector TargetLocationStage1;
	FVector TargetLocationStage2;
	
};

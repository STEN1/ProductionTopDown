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
	
	virtual void Tick(float DeltaSeconds) override;

	//virtual void Interact(bool Condition = true);

	void ActivateFromInteractObject(bool Condition);

	UPROPERTY(VisibleAnywhere, Category="Setup")
	UStaticMeshComponent* BaseMesh{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	UStaticMeshComponent* SpikeMesh{nullptr};
	UPROPERTY(EditAnywhere, Category="Setup")
	FVector Stage1Offset{10.f, 10.f, 10.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	FVector Stage2Offset{20.f, 20.f, 20.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	float Stage1Timer{1.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	float Stage2Timer{2.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	float Stage0Timer{3.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	float SpikeMoveSpeed{50.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	bool bLoop{true};
	UPROPERTY(EditAnywhere, Category="Setup")
	bool bStartSpikeOut{false};
	UPROPERTY(EditAnywhere, Category="Setup")
	bool bStartActivated{false};

protected:
	virtual void BeginPlay() override;
	
private:
	FVector StartLocation;
	FVector TargetLocationStage1;
	FVector TargetLocationStage2;

	float TickTimer{0.f};

	int32 SpikeState{0};	// 0 == Peaking out, 1 == Fully out, 2 == Reset/Hidden
};

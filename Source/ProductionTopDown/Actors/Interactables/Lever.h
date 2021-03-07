// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "Lever.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API ALever : public AInteractableBase
{
	GENERATED_BODY()

public:
	ALever();

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category="Setup")
	UStaticMeshComponent* LeverBase{nullptr};
	UPROPERTY(EditAnywhere, Category="Setup")
	UStaticMeshComponent* LeverHandle{nullptr};
	UPROPERTY(EditAnywhere, Category="Setup");
    float TargetYaw{60.f};
	UPROPERTY(EditAnywhere, Category="Setup");
	float TurnSpeed{100.f};
	UPROPERTY(EditInstanceOnly, Category="Setup")
	float TimeBeforeClose{1.0f};
	UPROPERTY(EditInstanceOnly, Category="Setup")
	bool bTimer{false};
	
	virtual void Interact() override;
	
	UPROPERTY(EditInstanceOnly, Category="Setup")
	class ADoorActor* DoorRef{nullptr};
	
private:
	FRotator StartRotation{0.f, 0.f, 0.f};
	FRotator TargetRotation{0.f, 0.f, 0.f};
	FTimerHandle CloseTimer;
	
	float ActivateTimer{0.f};
	bool bIsActivated{false};

	protected:
	virtual void BeginPlay() override;
};

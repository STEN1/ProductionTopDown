// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivatableBase.h"
#include "Components/BoxComponent.h"

#include "SpikeTrap.generated.h"



UCLASS()
class PRODUCTIONTOPDOWN_API ASpikeTrap : public AActivatableBase
{
	GENERATED_BODY()

public:
	ASpikeTrap();
	
	virtual void Tick(float DeltaSeconds) override;

	//virtual void Interact(bool Condition = true);

	virtual void Activate(bool On) override;

    UPROPERTY(EditAnywhere, Category="Sound")
	class USoundAttenuation* SoundAttenuation;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* HalfSpikeSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* FullExtendSound;
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* RedrawSound;
	
	UPROPERTY(VisibleAnywhere, Category="Setup")
	UStaticMeshComponent* BaseMesh{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	UStaticMeshComponent* SpikeMesh{nullptr};
	UPROPERTY(VisibleAnywhere, Category="Setup")
	UBoxComponent* BoxComponent{nullptr};
	UPROPERTY(EditAnywhere, Category="Setup")
	FVector Stage1Offset{10.f, 10.f, 10.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	FVector Stage2Offset{20.f, 20.f, 20.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	float StartDelay{-1.f};
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
	UPROPERTY(EditAnywhere, Category="Setup")
	bool bActivateOnTouch{false};

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

protected:
	virtual void BeginPlay() override;
	
private:
	FVector StartLocation;
	FVector TargetLocationStage1;
	FVector TargetLocationStage2;

	float TickTimer{0.f};

	bool bActivated{false};

	int32 SpikeState{0};	// 0 == Peaking out, 1 == Fully out, 2 == Reset/Hidden
};

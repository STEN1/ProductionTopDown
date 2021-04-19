// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "ProductionTopDown/Actors/Interactables/ActivatableBase.h"
#include "DoorActor.generated.h"

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	EaseOut	= 0			UMETA(DisplayName = "Ease out door movement"),
    Accelerate = 1		UMETA(DisplayName = "Accelerate door movement"),
    Constant = 2		UMETA(DisplayName = "Constant movement"),
};
UCLASS()
class PRODUCTIONTOPDOWN_API ADoorActor : public AActivatableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(bool On) override;
	void SetAlwaysMoving(bool AlwaysMoving);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	EDoorType DoorTypeOpen;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	EDoorType DoorTypeClose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ClosedParticle;
	
	FVector StartLocation;
	FRotator StartRotation;

	FVector TargetLocation;
	FRotator TargetRotation;

	bool bDoorOpen = false;
	
	FTimerHandle CloseTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	bool bStayOpen{false};
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float CloseDelay{-1.f};
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	ATriggerVolume* OpenTrigger;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float TargetYaw = 90.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float TargetXPos = 0.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float TargetYPos = 0.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float TargetZPos = 200.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float OpenSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float CloseSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	float MaxExpoSpeed = 1200.f;
	float ExpoSpeed{10.f};
	float DoorSpeed{0.f};
	const float Accel{9.8f};
	float CurrentYawOffset{0.f};

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void EaseOpenDoor(float DeltaTime);
	void EaseCloseDoor(float DeltaTime);
	void AccelOpenDoor(float DeltaTime);
	void AccelCloseDoor(float DeltaTime);
	
	void OnDoorOpened();
	void OnDoorClosed();

	

	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	USoundBase* ClosedSound;

	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	bool bAlwaysMoving{false};

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

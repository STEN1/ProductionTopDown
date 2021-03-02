// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Trace/Detail/LogScope.h"

#include "DoorActor.generated.h"
UENUM(BlueprintType)
enum class EDoorType : uint8
{
	EaseOut	= 0			UMETA(DisplayName = "Ease out door movement"),
    Accelerate = 1		UMETA(DisplayName = "Accelerate door movement"),
    Constant = 2		UMETA(DisplayName = "Constant movement"),
};
UCLASS()
class PRODUCTIONTOPDOWN_API ADoorActor : public AActor
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

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door Trigger", meta = (AllowPrivateAccess = "true"))
	ATriggerVolume* TriggerVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	EDoorType DoorTypeOpen;
	UPROPERTY(EditAnywhere, Category = "Door Settings", meta = (AllowPrivateAccess = "true"))
	EDoorType DoorTypeClose;
	
	FVector StartLocation;
	FRotator StartRotation;

	FVector TargetLocation;
	FRotator TargetRotation;

	bool bDoorOpen = false;

	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float TargetYaw = 90.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float TargetXPos = 0.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float TargetYPos = 0.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float TargetZPos = 200.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float OpenSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float CloseSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Door Settings")
	float MaxExpoSpeed = 1200.f;
	float ExpoSpeed{10.f};

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void EaseOpenDoor(float DeltaTime);
	void EaseCloseDoor(float DeltaTime);
	void AccelOpenDoor(float DeltaTime);
	void AccelCloseDoor(float DeltaTime);
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	

};

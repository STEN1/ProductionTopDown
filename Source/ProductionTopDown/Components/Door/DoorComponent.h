// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"

#include "DoorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRODUCTIONTOPDOWN_API UDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Door")
	UBoxComponent* DoorTrigger;
	
	FVector StartLocation;
	FRotator StartRotation;

	FVector TargetLocation;
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Door")
	float TargetYaw = 90.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float TargetXPos = 0.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float TargetYPos = 0.f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float TargetZPos = 200.f;

	void OpenDoor();
	void CloseDoor();
	void BeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};

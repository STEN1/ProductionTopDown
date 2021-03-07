// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "DoubleDoorsActor.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API ADoubleDoorsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoubleDoorsActor();

	UPROPERTY(EditAnywhere, Category="Setup")
	UStaticMeshComponent* LeftDoor{nullptr};
	UPROPERTY(EditAnywhere, Category="Setup")
	UStaticMeshComponent* RightDoor{nullptr};
	UPROPERTY(EditInstanceOnly, Category="Setup")
	ATriggerVolume* TriggerVolume{nullptr};

	UPROPERTY(EditAnywhere, Category="Setup")
	float TargetYaw{90.f};
	UPROPERTY(EditAnywhere, Category="Setup")
	float OpenSpeed{100.f};
	
	void OpenFromInteract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FRotator LeftStartRotation{0.f,0.f,0.f};
	FRotator RightStartRotation{0.f,0.f,0.f};	//Yaw must rotate
	FRotator TargetRotation{0.f, 0.f, 0.f};
	
	bool bIsOpen{false};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

};

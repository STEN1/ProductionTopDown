// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

class APuzzleController;

UCLASS()
class PRODUCTIONTOPDOWN_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlate();

	UPROPERTY(EditInstanceOnly, Category="Setup")
	TArray<class AActivatableBase*> ActivateActors;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsPressed{false};
	
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
    void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void ActivateLoop(bool On);

	FVector PressedPosition{0.f, 0.f, -5.f};
	FVector ReleasedPosition{FVector::ZeroVector};

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateFrame;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	ATriggerVolume* PlateTrigger;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};



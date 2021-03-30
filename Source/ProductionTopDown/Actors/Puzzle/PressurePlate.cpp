// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

#include "ProductionTopDown/Actors/Door/DoorActor.h"
#include "ProductionTopDown/Actors/Interactables/SpikeTrap.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = SceneComponent;
	PlateFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	PlateFrame->SetupAttachment(RootComponent);
	PlateButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	PlateButton->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	if(PlateTrigger)
	{
		PlateTrigger->OnActorBeginOverlap.AddDynamic(this, &APressurePlate::BeginOverlap);
		PlateTrigger->OnActorEndOverlap.AddDynamic(this, &APressurePlate::EndOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing TriggerVolume Reference: %s"), *GetHumanReadableName());
	}

	ReleasedPosition = PlateButton->GetRelativeLocation();
	
}

void APressurePlate::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	
	if (OtherActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		bIsPressed = true;
		PlateButton->SetRelativeLocation(PressedPosition);
		ActivateLoop(true);
	}
}

void APressurePlate::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		bIsPressed = false;
		PlateButton->SetRelativeLocation(ReleasedPosition);
		ActivateLoop(false);
	}
}

void APressurePlate::ActivateLoop(bool On)
{
	if (DoorActors.Num() != 0)
	{
		for (int i = 0; i < DoorActors.Num(); ++i)
    	{
    		DoorActors[i]->Activate(On);
    	}	
	}

	if (SpikeActors.Num() != 0)
	{
		for (int i = 0; i < SpikeActors.Num(); ++i)
		{
			SpikeActors[i]->Activate(On);
		}
	}

}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


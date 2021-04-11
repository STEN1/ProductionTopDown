// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"


#include "Components/CapsuleComponent.h"
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
	PlateTrigger = CreateDefaultSubobject<UBoxComponent>("PlateTrigger");
	PlateTrigger->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	if(PlateTrigger)
	{
		PlateTrigger->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::BeginOverlap);
		PlateTrigger->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::EndOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing TriggerVolume Reference: %s"), *GetHumanReadableName());
	}

	ReleasedPosition = PlateButton->GetRelativeLocation();
	
}

void APressurePlate::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		bIsPressed = true;
		PlateButton->SetRelativeLocation(PressedPosition);
		ActivateLoop(true);
	}
}

void APressurePlate::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		bIsPressed = false;
		PlateButton->SetRelativeLocation(ReleasedPosition);
		ActivateLoop(false);
	}
}

void APressurePlate::ActivateLoop(bool On)
{
	if (ActivateActors.Num() != 0)
	{
		for (int i = 0; i < ActivateActors.Num(); ++i)
    	{
    		ActivateActors[i]->Activate(On);
    	}	
	}

}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


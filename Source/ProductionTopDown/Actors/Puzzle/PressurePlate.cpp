// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "PuzzleController.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	PlateFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	PlateFrame->SetupAttachment(RootComponent);
	PlateButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	PlateButton->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	if(PlateTrigger2)
	{
		PlateTrigger2->OnActorBeginOverlap.AddDynamic(this, &APressurePlate::BeginOverlap);
		PlateTrigger2->OnActorEndOverlap.AddDynamic(this, &APressurePlate::EndOverlap);	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing TriggerVolume Reference: %s"), *GetHumanReadableName());
	}
	if (!PuzzleController)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing PuzzleController Reference: %s"), *GetHumanReadableName());
	}

	ReleasedPosition = PlateButton->GetRelativeLocation();
	
}

void APressurePlate::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bIsPressed = true;
	PlateButton->SetRelativeLocation(PressedPosition);
}

void APressurePlate::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	bIsPressed = false;
	PlateButton->SetRelativeLocation(ReleasedPosition);
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrap.h"

ASpikeTrap::ASpikeTrap()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	SpikeMesh->SetupAttachment(RootComponent);
	
}

void ASpikeTrap::BeginPlay()
{
	StartLocation = SpikeMesh->GetRelativeLocation();
	UE_LOG(LogTemp, Warning, TEXT("StartLocation: %f, %f, %f"), StartLocation.X, StartLocation.Y, StartLocation.Z);
}

void ASpikeTrap::Tick(float DeltaSeconds)
{
	
}

void ASpikeTrap::Interact(bool Condition)
{
}





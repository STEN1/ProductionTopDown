// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "ProductionTopDown/Actors/Door/DoorActor.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	LeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverBaseMesh"));
	LeverBase->SetupAttachment(RootComponent);
	LeverHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverHandleMesh"));
	LeverHandle->SetupAttachment(RootComponent);
}

void ALever::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bIsActivated)
	{
        	FRotator NewRotation = StartRotation;
        	NewRotation.Pitch = FMath::FInterpConstantTo(LeverHandle->GetRelativeRotation().Pitch, TargetRotation.Pitch, DeltaSeconds, TurnSpeed);
        	LeverHandle->SetRelativeRotation(NewRotation);
        
        	if (NewRotation.Pitch == TargetRotation.Pitch)
        	{
        		UE_LOG(LogTemp, Error, TEXT("Done Moving"));
        		SetActorTickEnabled(false);
        	}
	}
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	
	StartRotation = LeverHandle->GetRelativeRotation();
	TargetRotation.Pitch = StartRotation.Pitch - TargetYaw;
}

void ALever::Interact()
{
	Super::Interact();
	
	SetActorTickEnabled(true);
	bIsActivated = true;
	if (DoorRef)
	{
		DoorRef->OpenFromInteract();
	}
}
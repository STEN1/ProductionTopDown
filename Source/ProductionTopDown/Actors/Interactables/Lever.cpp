// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "ProductionTopDown/Actors/Door/DoorActor.h"
#include "ProductionTopDown/Actors/Interactables/SpikeTrap.h"

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
        if (bTimer)
        {
			ActivateTimer += DeltaSeconds;
			if (ActivateTimer >= TimeBeforeClose)
			{
				ActivateTimer = 0.f;
				Interact();
			}
        }

        if (NewRotation.Pitch == TargetRotation.Pitch)
        {
        	UE_LOG(LogTemp, Error, TEXT("Lever Down"));
            if (!bTimer)
            {
	            SetActorTickEnabled(false);
            }
        	
        }
	}
	else
	{
		FRotator NewRotation = TargetRotation;
		NewRotation.Pitch = FMath::FInterpConstantTo(LeverHandle->GetRelativeRotation().Pitch, StartRotation.Pitch, DeltaSeconds, TurnSpeed);
		LeverHandle->SetRelativeRotation(NewRotation);

		if (NewRotation.Pitch == StartRotation.Pitch)
		{
			UE_LOG(LogTemp, Error, TEXT("Lever Up"));
			SetActorTickEnabled(false);
		}
	}

	//UE_LOG(LogTemp, Error, TEXT("TICK IS STILL ON"));
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	
	StartRotation = LeverHandle->GetRelativeRotation();
	TargetRotation.Pitch = StartRotation.Pitch - TargetYaw;
	SetActorTickEnabled(false);
}

void ALever::Interact(bool Condition)
{
	Super::Interact();
	
	SetActorTickEnabled(true);
	bIsActivated = !bIsActivated;
	ActivateTimer = 0.f;
	
	if (DoorActors.Num() > 0)
	{
		for (int i = 0; i < DoorActors.Num(); ++i)
		{
			if (DoorActors[i])
			{
				DoorActors[i]->OpenFromInteract();
			}
		}
	}

	if (SpikeActors.Num() > 0)
	{
		for (int i = 0; i < SpikeActors.Num(); ++i)
		{
			if (SpikeActors[i])
			{
				SpikeActors[i]->ActivateFromInteractObject(false);
			}
		}
	}
}
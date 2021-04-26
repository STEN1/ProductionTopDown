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
	
	if (ActivateActors.Num() > 0 && !bDontActivate)
	{
		for (int i = 0; i < ActivateActors.Num(); ++i)
		{
			if (ActivateActors[i])
			{
				ActivateActors[i]->Activate(bIsActivated);
			}
		}
		if (bActivateOnlyOnce)
		{
			bDontActivate = !bDontActivate;
		}
	}
}
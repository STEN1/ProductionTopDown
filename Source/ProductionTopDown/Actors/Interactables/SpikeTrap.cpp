// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrap.h"

ASpikeTrap::ASpikeTrap()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	SpikeMesh->SetupAttachment(RootComponent);
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	
}

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = SpikeMesh->GetRelativeLocation();
	TargetLocationStage1 = StartLocation + Stage1Offset;
	TargetLocationStage2 = StartLocation + Stage2Offset;

	SetActorTickEnabled(bStartActivated);

	if (bStartSpikeOut)
	{
		SpikeMesh->SetRelativeLocation(TargetLocationStage2);
		TickTimer = Stage2Timer;
		SpikeState = 2;
	}
}

void ASpikeTrap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TickTimer += DeltaSeconds;
	if (TickTimer >= Stage1Timer && SpikeState == 0)
	{
		FVector NewLocation;
		NewLocation.Z = FMath::FInterpConstantTo(SpikeMesh->GetRelativeLocation().Z, TargetLocationStage1.Z, DeltaSeconds, SpikeMoveSpeed);
		SpikeMesh->SetRelativeLocation(NewLocation);
		if (NewLocation.Z == TargetLocationStage1.Z)
		{
			++SpikeState;
		}

	} else if (TickTimer >= Stage2Timer && SpikeState == 1)
	{
		FVector NewLocation;
		NewLocation.Z = FMath::FInterpConstantTo(SpikeMesh->GetRelativeLocation().Z, TargetLocationStage2.Z, DeltaSeconds, SpikeMoveSpeed);
		SpikeMesh->SetRelativeLocation(NewLocation);
		if (NewLocation.Z == TargetLocationStage2.Z)
		{
			++SpikeState;
            SetActorTickEnabled(bLoop);
		}

	} else if (TickTimer >= Stage0Timer && SpikeState == 2)
	{
		FVector NewLocation;
		NewLocation.Z = FMath::FInterpConstantTo(SpikeMesh->GetRelativeLocation().Z, StartLocation.Z, DeltaSeconds, SpikeMoveSpeed);
		SpikeMesh->SetRelativeLocation(NewLocation);
		if (NewLocation.Z == StartLocation.Z)
		{
			TickTimer = 0.f;
           	SpikeState = 0;
           	SetActorTickEnabled(bLoop);
			//SoundFX can go here!
		}
	}
}

void ASpikeTrap::ActivateFromInteractObject(bool Condition)
{
	SetActorTickEnabled(true);
	if (Condition)
	{
		bLoop = true;
	}
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrap.h"


#include "Components/CapsuleComponent.h"
#include "ProductionTopDown/Character/CharacterBase.h"
#include "ProductionTopDown/Components/InteractComponent.h"

ASpikeTrap::ASpikeTrap()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	SpikeMesh->SetupAttachment(RootComponent);
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	
}



void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = SpikeMesh->GetRelativeLocation();
	TargetLocationStage1 = StartLocation + Stage1Offset;
	TargetLocationStage2 = StartLocation + Stage2Offset;

	SetActorTickEnabled(bStartActivated);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::OnComponentBeginOverlap);
	BoxComponent->SetGenerateOverlapEvents(bActivateOnTouch);

	if (bStartSpikeOut)
	{
		SpikeMesh->SetRelativeLocation(TargetLocationStage2);
		TickTimer = Stage2Timer;
		SpikeState = 2;
	}

	if (StartDelay <= 0)
	{
		bActivated = true;
	}
}

void ASpikeTrap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TickTimer += DeltaSeconds;
	if (TickTimer >= StartDelay || bActivated)
	{
		if (!bActivated)
		{
			bActivated = true;
			TickTimer = 0.f;
		}
		
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
				if (bStartSpikeOut)
				{
					SetActorTickEnabled(bLoop);
				}
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
}

void ASpikeTrap::Activate(bool On)
{
	SetActorTickEnabled(true);
	if (On)
	{
		bLoop = true;
	}
}

void ASpikeTrap::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacterBase::StaticClass()) && OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		Activate(false);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Camera/CameraComponent.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	if (!TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("No TriggerVolume on %s"), *GetHumanReadableName());
		return;
	}
	SetActorTickEnabled(true);
	TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ADoorActor::BeginOverlap);
	TriggerVolume->OnActorEndOverlap.AddDynamic(this, &ADoorActor::EndOverlap);
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	
	TargetLocation = FVector(
        StartLocation.X + TargetXPos,
        StartLocation.Y + TargetYPos,
        StartLocation.Z + TargetZPos);
	TargetRotation = FRotator(
        StartRotation.Pitch,
        StartRotation.Yaw + TargetYaw,
        StartRotation.Roll);
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bDoorOpen)
	{
		switch(DoorTypeOpen)
		{
			case EDoorType::Constant:
				OpenDoor(DeltaTime);
				break;
			case EDoorType::EaseOut:
				EaseOpenDoor(DeltaTime);
				break;
			case EDoorType::Accelerate:
				AccelOpenDoor(DeltaTime);
				break;
		}
	}
	else
	{
		switch(DoorTypeClose)
		{
			case EDoorType::Constant:
				CloseDoor(DeltaTime);
				break;
			case EDoorType::EaseOut:
				EaseCloseDoor(DeltaTime);
				break;
			case EDoorType::Accelerate:
				AccelCloseDoor(DeltaTime);
				break;
		}
	}

}

void ADoorActor::OpenDoor(float DeltaTime)
{
	FVector NewLocation = TargetLocation;
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, TargetLocation.Z, DeltaTime, OpenSpeed);
	SetActorLocation(NewLocation);
}

void ADoorActor::CloseDoor(float DeltaTime)
{
	FVector NewLocation = StartLocation;
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, StartLocation.Z, DeltaTime, CloseSpeed);
	SetActorLocation(NewLocation);
}

void ADoorActor::EaseOpenDoor(float DeltaTime)
{
	FVector NewLocation = TargetLocation;
	NewLocation.Z = FMath::InterpExpoOut(GetActorLocation().Z, TargetLocation.Z, DeltaTime * OpenSpeed * 0.005f);
	SetActorLocation(NewLocation);
}

void ADoorActor::EaseCloseDoor(float DeltaTime)
{
	FVector NewLocation = StartLocation;
	NewLocation.Z = FMath::InterpExpoOut(GetActorLocation().Z, StartLocation.Z, DeltaTime * OpenSpeed * 0.005f);
	SetActorLocation(NewLocation);
}

void ADoorActor::AccelOpenDoor(float DeltaTime)
{
	FVector NewLocation = StartLocation;
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, TargetLocation.Z, DeltaTime, OpenSpeed + ExpoSpeed);
	SetActorLocation(NewLocation);

	ExpoSpeed = FMath::Clamp(FMath::Pow(ExpoSpeed, 1.025f),0.f,MaxExpoSpeed);

	if (NewLocation.Z == TargetLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door at Top"))

		//Particle FX and Sound here***
		
		SetActorTickEnabled(false);
	}

}

void ADoorActor::AccelCloseDoor(float DeltaTime)
{
	FVector NewLocation = TargetLocation;
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, StartLocation.Z, DeltaTime, OpenSpeed + ExpoSpeed);
	SetActorLocation(NewLocation);
	
	ExpoSpeed = FMath::Clamp(FMath::Pow(ExpoSpeed, 1.025f),0.f,MaxExpoSpeed);

	if (NewLocation.Z == StartLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door at Floor"))

		//Particle FX and Sound here***
		
		SetActorTickEnabled(false);
	}
}

void ADoorActor::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (/*APlayerPawn* Player = */Cast<APlayerCharacter>(OtherActor))
	{
		bDoorOpen = true;
		ExpoSpeed = 10.f;
		SetActorTickEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("DOOR BEGINOverlappedActor: %s"), *OverlappedActor->GetHumanReadableName());
		UE_LOG(LogTemp, Warning, TEXT("DOOR BEGINOtherActor: %s"), *OtherActor->GetHumanReadableName());
	}
}

void ADoorActor::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (/*APlayerPawn* Player = */Cast<APlayerCharacter>(OtherActor))
	{
		bDoorOpen = false;
		ExpoSpeed = 10.f;
		SetActorTickEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("DOOR ENDOverlappedActor: %s"), *OverlappedActor->GetHumanReadableName());
		UE_LOG(LogTemp, Warning, TEXT("DOOR ENDOtherActor: %s"), *OtherActor->GetHumanReadableName());
	}
}


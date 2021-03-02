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

	switch (DoorType)
	{
		case EDoorType::Constant:
			if (bDoorOpen)
			{
				OpenDoor(DeltaTime);
			}
			else
			{
				CloseDoor(DeltaTime);
			}
			break;
		case EDoorType::Accelerate:
			break;
		case EDoorType::EaseOut:
			if (bDoorOpen)
			{
				EaseOpenDoor(DeltaTime);
			}
			else
			{
				EaseCloseDoor(DeltaTime);
			}
			break;
		default:
			break;
	}
}

void ADoorActor::OpenDoor(float DeltaTime)
{
	FRotator NewRotation = TargetRotation;
	NewRotation.Yaw = FMath::FInterpConstantTo(GetActorRotation().Yaw, TargetRotation.Yaw, DeltaTime, OpenSpeed);
	SetActorRotation(NewRotation);

	FVector NewLocation = TargetLocation;
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, TargetLocation.Z, DeltaTime, OpenSpeed);
	SetActorLocation(NewLocation);
}

void ADoorActor::CloseDoor(float DeltaTime)
{
	FRotator NewRotation = StartRotation;
	NewRotation.Yaw = FMath::FInterpConstantTo(GetActorRotation().Yaw, StartRotation.Yaw, DeltaTime, CloseSpeed);
	SetActorRotation(NewRotation);

	FVector NewLocation = StartLocation;
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, StartLocation.Z, DeltaTime, CloseSpeed);
	SetActorLocation(NewLocation);
}

void ADoorActor::EaseOpenDoor(float DeltaTime)
{
	FRotator NewRotation = TargetRotation;
	NewRotation.Yaw = FMath::Lerp(GetActorRotation().Yaw, TargetRotation.Yaw, DeltaTime * OpenSpeed * 0.01f);
	SetActorRotation(NewRotation);
}

void ADoorActor::EaseCloseDoor(float DeltaTime)
{
	FRotator NewRotation = StartRotation;
	NewRotation.Yaw = FMath::Lerp(GetActorRotation().Yaw, StartRotation.Yaw, DeltaTime * CloseSpeed * 0.01f);
	SetActorRotation(NewRotation);
}

void ADoorActor::AccelOpenDoor(float DeltaTime)
{
}

void ADoorActor::AccelCloseDoor(float DeltaTime)
{
}


void ADoorActor::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (/*APlayerPawn* Player = */Cast<APlayerCharacter>(OtherActor))
	{
		bDoorOpen = true;
		UE_LOG(LogTemp, Warning, TEXT("DOOR BEGINOverlappedActor: %s"), *OverlappedActor->GetHumanReadableName());
		UE_LOG(LogTemp, Warning, TEXT("DOOR BEGINOtherActor: %s"), *OtherActor->GetHumanReadableName());
	}
}

void ADoorActor::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (/*APlayerPawn* Player = */Cast<APlayerCharacter>(OtherActor))
	{
		bDoorOpen = false;
		UE_LOG(LogTemp, Warning, TEXT("DOOR ENDOverlappedActor: %s"), *OverlappedActor->GetHumanReadableName());
		UE_LOG(LogTemp, Warning, TEXT("DOOR ENDOtherActor: %s"), *OtherActor->GetHumanReadableName());
	}
}


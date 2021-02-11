// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorComponent.h"

// Sets default values for this component's properties
UDoorComponent::UDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTrigger"));
	

	// ...
}


// Called when the game starts
void UDoorComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	SetComponentTickEnabled(false);
	StartLocation = GetOwner()->GetActorLocation();
	StartRotation = GetOwner()->GetActorRotation();

	DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &UDoorComponent::BeginOverlap);
	DoorTrigger->SetWorldLocation(StartLocation);
	DoorTrigger->SetWorldRotation(StartRotation);
	

	TargetLocation = FVector(
		StartLocation.X + TargetXPos,
		StartLocation.Y + TargetYPos,
		StartLocation.Z + TargetZPos);
	TargetRotation = FRotator(
		StartRotation.Pitch,
		StartRotation.Yaw + TargetYaw,
		StartRotation.Roll);
	/*
	UE_LOG(LogTemp, Warning, TEXT("%s Location: %s"),
		*GetOwner()->GetHumanReadableName(),
		*GetOwner()->GetActorLocation().ToCompactString());
	UE_LOG(LogTemp, Warning, TEXT("%s Rotation: %s"),
        *GetOwner()->GetHumanReadableName(),
        *GetOwner()->GetActorRotation().ToCompactString());
	*/
}


// Called every frame
void UDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDoorComponent::OpenDoor()
{
}

void UDoorComponent::CloseDoor()
{
}

void UDoorComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}






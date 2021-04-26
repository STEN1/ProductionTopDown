// Fill out your copyright notice in the Description page of Project Settings.


#include "ScentComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UScentComponent::UScentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnScent)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UScentComponent::SpawnScent, SpawnRate, true);
	}
	
	SetComponentTickEnabled(bDrawDebugScent);
}

void UScentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void UScentComponent::SpawnScent()
{
	FVector NewScent{GetOwner()->GetActorLocation()};
	ScentArray.Add(NewScent);

	if (ScentArray.Num() >= MaxScentActors+1)
	{
        ScentArray.RemoveAt(0);
	}

}

// Called every frame
void UScentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int i = 0; i < ScentArray.Num(); ++i)
    {
        DrawDebugSphere(GetWorld(), ScentArray[i], 50.f, 8,FColor::Green);
    }	
}


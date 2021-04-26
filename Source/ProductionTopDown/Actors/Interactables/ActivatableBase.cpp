// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivatableBase.h"

// Sets default values
AActivatableBase::AActivatableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AActivatableBase::Activate(bool On)
{
	if (On)
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deactivated!"));
	}
	
}

// Called when the game starts or when spawned
void AActivatableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActivatableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


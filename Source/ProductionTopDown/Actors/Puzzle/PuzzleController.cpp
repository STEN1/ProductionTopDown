// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleController.h"

// Sets default values
APuzzleController::APuzzleController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzleController::PrintTest(int32 State)
{
	UE_LOG(LogTemp, Warning, TEXT("PrintTest: %i"), State);
}

void APuzzleController::PressurePlateTriggered()
{
	
}

// Called when the game starts or when spawned
void APuzzleController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


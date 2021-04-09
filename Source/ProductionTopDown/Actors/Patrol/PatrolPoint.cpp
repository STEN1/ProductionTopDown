// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"
#include "Components/SphereComponent.h"

// Sets default values
APatrolPoint::APatrolPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

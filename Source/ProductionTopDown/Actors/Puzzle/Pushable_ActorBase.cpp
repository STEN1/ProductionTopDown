// Fill out your copyright notice in the Description page of Project Settings.


#include "Pushable_ActorBase.h"

// Sets default values
APushable_ActorBase::APushable_ActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PushableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PushableMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APushable_ActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APushable_ActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


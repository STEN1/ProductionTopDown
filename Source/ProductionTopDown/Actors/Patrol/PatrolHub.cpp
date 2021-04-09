// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolHub.h"
#include "DrawDebugHelpers.h"
#include "PatrolPoint.h"
#include "Components/BoxComponent.h"

// Sets default values
APatrolHub::APatrolHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	SetActorTickEnabled(bDrawLines);

}

// Called when the game starts or when spawned
void APatrolHub::BeginPlay()
{
	Super::BeginPlay();

	if (PatrolPoints.Num() < 2)
	{
		UE_LOG(LogTemp, Error, TEXT("PATROL ROUTE NEEDS ATLEAST 2 POINTS"));
		SetActorTickEnabled(false);
	}
	
}

void APatrolHub::Tick(float DeltaTime)
{
	if (bDrawLines && PatrolPoints.Num() > 1)
	{
		for (int i = 1; i < PatrolPoints.Num(); ++i)
        {
        	DrawDebugLine(GetWorld(), PatrolPoints[i]->GetActorLocation(), PatrolPoints[i-1]->GetActorLocation(), FColor::Silver, false, -1, 0, 5);
        }
		DrawDebugLine(GetWorld(), PatrolPoints[PatrolPoints.Num()-1]->GetActorLocation(), PatrolPoints[0]->GetActorLocation(), FColor::Silver, false, -1, 0, 5);
	}

	
}


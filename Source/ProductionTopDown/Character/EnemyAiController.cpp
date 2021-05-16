// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAiController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAiController::AEnemyAiController()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AEnemyAiController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Behaviour tree not found"))
	}

	if(EnemyBlackBoard)	EnemyBlackBoard = GetBlackboardComponent();
	if(PlayerPawn && EnemyBlackBoard)
	{
		EnemyBlackBoard->SetValueAsObject(TEXT("Player"), PlayerPawn->GetOwner());
		UE_LOG(LogTemp, Error, TEXT("Playerpawnfound "));
	}
	if(PlayerPawn && PlayerPawn->GetOwner() &&EnemyBlackBoard)
	{
		EnemyBlackBoard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetOwner()->GetActorLocation());
		UE_LOG(LogTemp, Error, TEXT("Playerpawnfound"));
	}
}

void AEnemyAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void AEnemyAiController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//clear Timers
	
}

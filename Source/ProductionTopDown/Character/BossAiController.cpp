// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAiController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossAiController::ABossAiController()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ABossAiController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if(BossBehaviorTree)
	{
		RunBehaviorTree(BossBehaviorTree);
	};
	BossBlackBoard = GetBlackboardComponent();
	if(PlayerPawn && BossBlackBoard) BossBlackBoard->SetValueAsObject(TEXT("Player"), PlayerPawn->GetOwner());
}

void ABossAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(PlayerPawn && BossBlackBoard)BossBlackBoard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
}

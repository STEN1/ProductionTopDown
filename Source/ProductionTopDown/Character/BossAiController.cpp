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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Behaviour tree not found"))
	}
	
	BossBlackBoard = GetBlackboardComponent();
	if(PlayerPawn && BossBlackBoard) BossBlackBoard->SetValueAsObject(TEXT("Player"), PlayerPawn->GetOwner());
	if(BossBlackBoard) BossBlackBoard->SetValueAsVector(TEXT("RoomCenter"), GetOwner()->GetActorLocation()); // Location to use spin attack
}

void ABossAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(BossBlackBoard)
	{
		if(PlayerPawn)BossBlackBoard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		
		if(PlayerPawn && LineOfSightTo(PlayerPawn)) BossBlackBoard->SetValueAsBool(TEXT("HasLineOfSight"), true);
		else BossBlackBoard->SetValueAsBool(TEXT("HasLineOfSight"), false);
		
		if(GetOwner()->GetDistanceTo(PlayerPawn) < 200) BossBlackBoard->SetValueAsBool(TEXT("InAttackRange"), true);
		else BossBlackBoard->SetValueAsBool(TEXT("InAttackRange"), false);
	}
	
}

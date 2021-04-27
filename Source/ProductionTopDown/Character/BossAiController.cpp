// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAiController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProductionTopDown/Components/HealthComponent.h"
#include "FirstBoss.h"

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
	if(BossBlackBoard && GetPawn()) BossBlackBoard->SetValueAsVector(TEXT("RoomCenter"), GetPawn()->GetActorLocation()); // Location to use spin attack

	Bossptr = Cast<AFirstBoss>(GetPawn());
	
}

void ABossAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(BossBlackBoard && Bossptr)
	{
		if(PlayerPawn)BossBlackBoard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		
		if(PlayerPawn && LineOfSightTo(PlayerPawn)) BossBlackBoard->SetValueAsBool(TEXT("HasLineOfSight"), true);
		else BossBlackBoard->SetValueAsBool(TEXT("HasLineOfSight"), false);
		
		if(GetPawn() &&  PlayerPawn && GetPawn()->GetDistanceTo(PlayerPawn) < 300) BossBlackBoard->SetValueAsBool(TEXT("InAttackRange"), true);
		else if(Bossptr->GetEnemyState() != EBossState::NormalAttack) BossBlackBoard->SetValueAsBool(TEXT("InAttackRange"), false);

		//nuke room every 30% of hp
		//walk to middle. use fidget spinner
		UHealthComponent* HealthComponent = GetPawn()->FindComponentByClass<UHealthComponent>();
		if (HealthComponent)
		{
			
			CurrentHP =  HealthComponent->GetHealth();
			const float BossHpPercent = CurrentHP / HealthComponent->GetDefaultHealth();
			//UE_LOG(LogTemp, Error, TEXT("BossHPPercent %f"),BossHpPercent);
			if(BossHpPercent <= 0.5f )
			{
				//UE_LOG(LogTemp, Error, TEXT("Fidget time!!!!!"));
				BossBlackBoard->SetValueAsBool(TEXT("UseFidgetMove"), true);
			}
			
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Healthcomponent not found"));
		
		
	}
	
}

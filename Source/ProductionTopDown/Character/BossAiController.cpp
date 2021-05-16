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
	UE_LOG(LogTemp, Error, TEXT("GO kys "));
	
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
	if(PlayerPawn && BossBlackBoard)
	{
		BossBlackBoard->SetValueAsObject(TEXT("Player"), PlayerPawn->GetOwner());
		UE_LOG(LogTemp, Error, TEXT("Playerpawnfound "));
	}
	if(BossBlackBoard && GetPawn())
	{
		BossBlackBoard->SetValueAsVector(TEXT("RoomCenter"), GetPawn()->GetActorLocation()); // Location to use spin attack
		UE_LOG(LogTemp, Error, TEXT("Spawn Location Found"));
	}
	
	
}

void ABossAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FirstTick && BossBlackBoard && GetPawn())
	{
		SpawnLocation = GetPawn()->GetActorLocation();
		Bossptr = Cast<AFirstBoss>(GetPawn());
		
		FirstTick = false;

		GetWorld()->GetTimerManager().SetTimer(GetZValueTimer,[this](){
				SpawnLocation.Z = GetPawn()->GetActorLocation().Z;
				BossBlackBoard->SetValueAsVector(TEXT("RoomCenter"), SpawnLocation); // Location to use spin attack
			},1.5f,false
		);
	}
		
	if(BossBlackBoard && Bossptr)
	{
		if(PlayerPawn)
		{
			BossBlackBoard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		}
		if(PlayerPawn && LineOfSightTo(PlayerPawn) && PlayerPawn->GetActorLocation().Z < Bossptr->GetActorLocation().Z + 20.f)
			BossBlackBoard->SetValueAsBool(TEXT("HasLineOfSight"), true);
		else
			BossBlackBoard->SetValueAsBool(TEXT("HasLineOfSight"), false);

		if(Bossptr->GetAttackCounter() <= 0)
		{
			BossBlackBoard->SetValueAsBool(TEXT("ShootFireball"), true);
		}
		else
		{
			BossBlackBoard->SetValueAsBool(TEXT("ShootFireball"), false);
		}
		
		if(GetPawn() &&  PlayerPawn && GetPawn()->GetDistanceTo(PlayerPawn) < 300.f) BossBlackBoard->SetValueAsBool(TEXT("InAttackRange"), true);
		else if(Bossptr->GetEnemyState() != EBossState::NormalAttack) BossBlackBoard->SetValueAsBool(TEXT("InAttackRange"), false);

		
		
		//nuke room when hp is at 30%
		//walk to middle. use fidget spinner
		UHealthComponent* HealthComponent = GetPawn()->FindComponentByClass<UHealthComponent>();
		if (HealthComponent)
		{
			
			CurrentHP =  HealthComponent->GetHealth();
			const float BossHpPercent = CurrentHP / HealthComponent->GetDefaultHealth();
			//UE_LOG(LogTemp, Error, TEXT("BossHPPercent %f"),BossHpPercent);
			if(BossHpPercent <= 0.3f )
			{
				//UE_LOG(LogTemp, Error, TEXT("Fidget time!!!!!"));
				BossBlackBoard->SetValueAsBool(TEXT("UseFidgetMove"), true);
			}
			
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Healthcomponent not found"));
		
		
	}
	
}

void ABossAiController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(GetZValueTimer);
}

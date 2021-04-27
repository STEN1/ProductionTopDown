// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_HalfChargeState.h"
#include "FirstBoss.h"
#include "AIController.h"

UBTT_HalfChargeState::UBTT_HalfChargeState()
{
	NodeName = TEXT("Set Half Charge");
}

EBTNodeResult::Type UBTT_HalfChargeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}
	
	// Set boss state

	AFirstBoss* Bossptr = Cast<AFirstBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(Bossptr)
	{
		Bossptr->SetEnemyState(EBossState::HalfCharged);
	}
	
	return EBTNodeResult::Succeeded;
}

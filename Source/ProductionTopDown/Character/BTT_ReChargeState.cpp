// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ReChargeState.h"
#include "FirstBoss.h"
#include "AIController.h"

UBTT_ReChargeState::UBTT_ReChargeState()
{
	NodeName = TEXT("Recharge State");
}

EBTNodeResult::Type UBTT_ReChargeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}
	
	// Resets boss state

	AFirstBoss* Bossptr = Cast<AFirstBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(Bossptr)
	{
		Bossptr->SetEnemyState(EBossState::ChargeAttack);
	}
	
	return EBTNodeResult::Succeeded;
}

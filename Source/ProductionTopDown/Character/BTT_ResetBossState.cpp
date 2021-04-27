// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ResetBossState.h"

#include "AIController.h"
#include "FirstBoss.h"

UBTT_ResetBossState::UBTT_ResetBossState()
{
	NodeName = TEXT("Boss State to Moving");
}

EBTNodeResult::Type UBTT_ResetBossState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		Bossptr->SetEnemyState(EBossState::Moving);
	}
	
	return EBTNodeResult::Succeeded;
}

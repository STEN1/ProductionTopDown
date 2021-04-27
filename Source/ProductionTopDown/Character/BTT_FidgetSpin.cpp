// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FidgetSpin.h"
#include "AIController.h"
#include "FirstBoss.h"

UBTT_FidgetSpin::UBTT_FidgetSpin()
{
	NodeName = TEXT("Use fidgetSpin attack");
}

EBTNodeResult::Type UBTT_FidgetSpin::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}
	 // move to center of room. use fidget spinner
	
	AFirstBoss* Bossptr = Cast<AFirstBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(Bossptr)
	{
		Bossptr->FidgetSpinAttack();
		Bossptr->SetEnemyState(EBossState::BigClearRoomAttack);
	}

	return EBTNodeResult::Succeeded;
}



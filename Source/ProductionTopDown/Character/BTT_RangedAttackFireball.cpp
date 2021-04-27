// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RangedAttackFireball.h"

UBTT_RangedAttackFireball::UBTT_RangedAttackFireball()
{
	NodeName = TEXT("FireBall Attack");
}

EBTNodeResult::Type UBTT_RangedAttackFireball::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}
	
	// Move to center of room shoot fireballs
	
	UE_LOG(LogTemp, Warning, TEXT("Shooting Fireball"))

	return EBTNodeResult::Succeeded;
}


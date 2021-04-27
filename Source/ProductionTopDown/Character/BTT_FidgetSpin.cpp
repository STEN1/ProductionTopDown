// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FidgetSpin.h"

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
	
	UE_LOG(LogTemp, Error, TEXT("Attackkiiinnngngngg scrubers"))

	return EBTNodeResult::Succeeded;
}



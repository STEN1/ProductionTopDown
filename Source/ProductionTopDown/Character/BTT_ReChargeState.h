// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ReChargeState.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API UBTT_ReChargeState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTT_ReChargeState();
	
	public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

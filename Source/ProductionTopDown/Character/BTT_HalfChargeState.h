// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_HalfChargeState.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API UBTT_HalfChargeState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTT_HalfChargeState();

	public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

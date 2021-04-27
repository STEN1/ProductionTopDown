// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FidgetSpin.generated.h"



UCLASS()
class PRODUCTIONTOPDOWN_API UBTT_FidgetSpin : public UBTTaskNode
{
	GENERATED_BODY()

	public:
	
	UBTT_FidgetSpin();

	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	
	protected:
	
	private:
	
};

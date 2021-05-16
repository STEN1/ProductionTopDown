// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_EnemyMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API UBTT_EnemyMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTT_EnemyMeleeAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

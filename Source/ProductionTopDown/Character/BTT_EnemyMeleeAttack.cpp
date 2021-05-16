// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyMeleeAttack.h"
#include "EnemyAiBase.h"
#include "AIController.h"

UBTT_EnemyMeleeAttack::UBTT_EnemyMeleeAttack()
{
	NodeName = TEXT("MeeleAttack");
}

EBTNodeResult::Type UBTT_EnemyMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
		Super::ExecuteTask(OwnerComp, NodeMemory);
    
    	if (!OwnerComp.GetAIOwner())
    	{
    		return EBTNodeResult::Failed;
    	}
    	
    	// slice melee attack
    	
    	//UE_LOG(LogTemp, Warning, TEXT("Slishing with sword"))
    
    	AEnemyAiBase* Enemyptr = Cast<AEnemyAiBase>(OwnerComp.GetAIOwner()->GetPawn());
    	if(Enemyptr)
    	{
    		Enemyptr->SetEnemyState(EEnemyAiState::MeleeAttack);
    	}
    	
    	return EBTNodeResult::Succeeded;
}

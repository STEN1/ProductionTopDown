// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MeeleAttack.h"

#include "AIController.h"
#include "FirstBoss.h"

UBTT_MeeleAttack::UBTT_MeeleAttack()
{
	NodeName = TEXT("MeeleAttack");
}

EBTNodeResult::Type UBTT_MeeleAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}
	
	// slice melee attack
	
	//UE_LOG(LogTemp, Warning, TEXT("Slishing with sword"))

	AFirstBoss* Bossptr = Cast<AFirstBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(Bossptr)
	{
		Bossptr->SetEnemyState(EBossState::NormalAttack);
		
	}
	
	return EBTNodeResult::Succeeded;
}



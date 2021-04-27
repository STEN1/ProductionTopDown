// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RangedAttackFireball.h"
#include "AIController.h"
#include "FirstBoss.h"


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
	
	//UE_LOG(LogTemp, Warning, TEXT("Shooting Fireball"))

	AFirstBoss* Bossptr = Cast<AFirstBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(Bossptr)
	{
		Bossptr->ShootFireBall();
		Bossptr->SetEnemyState(EBossState::RangeAttack);
	}
	return EBTNodeResult::Succeeded;
}


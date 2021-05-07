// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetMoveSpeed.h"
#include "AIController.h"
#include "FirstBoss.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_SetMoveSpeed::UBTT_SetMoveSpeed()
{
	NodeName = TEXT("Set Run Speed");
}

EBTNodeResult::Type UBTT_SetMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner())
	{
		return EBTNodeResult::Failed;
	}
	
	// Resets boss state

	AFirstBoss* Bossptr = Cast<AFirstBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(Bossptr)
	{
		Bossptr->SetEnemyState(EBossState::Running);
		Bossptr->GetCharacterMovement()->MaxWalkSpeed = 700;
	}
	
	return EBTNodeResult::Succeeded;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FirstBoss.h"
#include "BossAiController.generated.h"


/**
 * 
 */
UCLASS()


class PRODUCTIONTOPDOWN_API ABossAiController : public AAIController
{
	GENERATED_BODY()
	ABossAiController();
	
	public:
	virtual void BeginPlay() override;
	virtual void Tick (float DeltaSeconds) override;

	protected:
	
	
	private:
	APawn* PlayerPawn;
	AFirstBoss* Bossptr;
	
	float CurrentHP;
	bool HaveEnteredFidgetPhase{false};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BossBehaviorTree;
	class UBlackboardComponent* BossBlackBoard;
};


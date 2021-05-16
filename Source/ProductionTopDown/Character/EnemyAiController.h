// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAiController.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API AEnemyAiController : public AAIController
{
	GENERATED_BODY()
	AEnemyAiController();
	
	public:
	virtual void BeginPlay() override;
	virtual void Tick (float DeltaSeconds) override;

	protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* EnemyBehaviorTree;
	class UBlackboardComponent* EnemyBlackBoard;

	APawn* PlayerPawn;
};

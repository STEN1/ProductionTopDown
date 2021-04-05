// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolHub.generated.h"

class APatrolPoint;

UCLASS()
class PRODUCTIONTOPDOWN_API APatrolHub : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolHub();

	class UBoxComponent* BoxComponent{nullptr};

	UPROPERTY(EditAnywhere)
	TArray<APatrolPoint*> PatrolPoints;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    bool bDrawLines{false};

public:
	virtual void Tick(float DeltaTime) override;

};

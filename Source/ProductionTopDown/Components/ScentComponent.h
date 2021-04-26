// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScentComponent.generated.h"

class AScentActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRODUCTIONTOPDOWN_API UScentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScentComponent();

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> ScentArray;

	UPROPERTY(EditAnywhere)
	float SpawnRate{1.f};
	UPROPERTY(EditAnywhere)
	int32 MaxScentActors{5};
	UPROPERTY(EditAnywhere)
	bool bSpawnScent{true};
	UPROPERTY(EditAnywhere)
	bool bDrawDebugScent{false};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void SpawnScent();

private:
	FTimerHandle SpawnTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

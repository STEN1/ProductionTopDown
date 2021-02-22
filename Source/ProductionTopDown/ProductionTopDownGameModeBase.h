// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProductionTopDownGameModeBase.generated.h"

class APlayerPawn;
/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API AProductionTopDownGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthUI(float Health, float DefaultHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaminaUI(float Stamina, float DefaultStamina);
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);

	void ActorDied(AActor* DeadActor);
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	
private:
	APlayerPawn* Player;

	UFUNCTION()
	int32 GetPlayerDefaultHealth() const;
	UFUNCTION()
	int32 GetPlayerHealth() const;
	UFUNCTION()
	int32 GetPlayerDefaultStamina() const;
	UFUNCTION()
	int32 GetPlayerStamina() const;
};

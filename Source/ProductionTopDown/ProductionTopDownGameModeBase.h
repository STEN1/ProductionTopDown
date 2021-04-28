// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"



#include "Character/PlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "ProductionTopDownGameModeBase.generated.h"

/**
 * 
 */
class APlayerCharacter;
UCLASS()
class PRODUCTIONTOPDOWN_API AProductionTopDownGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Pause();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateNumberOfHealthPots(int32 NumberOfHealthPots);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealthUI(float Health, float DefaultHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaminaUI(float Stamina, float DefaultStamina);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventoryUI(int32 Slot, UTexture2D* ItemImage);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInventoryUICurrentSlot(int32 CurrentSlot);
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayTextBox();

	UPROPERTY(BlueprintReadOnly)
	FString TextBoxString{""};
	
	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();
	
private:
	UPROPERTY()
	APlayerCharacter* Player;

	UFUNCTION()
	float GetPlayerDefaultHealth() const;
	UFUNCTION()
	float GetPlayerHealth() const;
	UFUNCTION()
	float GetPlayerDefaultStamina() const;
	UFUNCTION()
	float GetPlayerStamina() const;


};

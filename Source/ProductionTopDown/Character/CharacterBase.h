// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UStaminaComponent;
class UHealthComponent;
UCLASS()
class PRODUCTIONTOPDOWN_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Functions
	virtual bool Attack();
	virtual bool Dash();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TriggerDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnAttackParticle();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnDeathParticle();
protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

private:
	//variables
	UPROPERTY(EditAnywhere, Category="Particle Effects")
	UParticleSystem* DeathParticle;
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	USoundBase* DeathSound;



};

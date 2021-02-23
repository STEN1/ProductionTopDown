// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "ProductionTopDown/Components/HealthComponent.h"
#include "ProductionTopDown/Components/StaminaComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>("Stamina Component");
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

bool ACharacterBase::Attack()
{
	if (StaminaComponent)
	{
		if (StaminaComponent->GetStamina() < StaminaComponent->GetAttackCost())
		{
			return false;
		}
		// Subtracts the stamina cost from current stamina.
		StaminaComponent->Attack();
	}
	return true;
}

bool ACharacterBase::Dash()
{
	if (StaminaComponent)
	{
		if (StaminaComponent->GetStamina() < StaminaComponent->GetDashCost())
		{
			return false;
		}
		// Subtracts the stamina cost from current stamina.
		StaminaComponent->Dash();
	}
	return true;
}


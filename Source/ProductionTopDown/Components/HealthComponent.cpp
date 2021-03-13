// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

int32 UHealthComponent::GetHealth() const
{
	return Health;
}

int32 UHealthComponent::GetDefaultHealth() const
{
	return DefaultHealth;
}



// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = DefaultHealth;
	HealthPercentage = Health / DefaultHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDmg);
	GameModeRef = Cast<AProductionTopDownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	// ...
	
}

void UHealthComponent::TakeDmg(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	HealthPercentage = Health / DefaultHealth;
	
	if (GetOwner()->IsA(APlayerCharacter::StaticClass()))
	{
		GameModeRef->UpdateHealthUI(Health, DefaultHealth);
	}
	if (Health <= 0)
	{
		// probably just call some kind of "HandleDeath()" function on the damaged actor.
		// we need to have a virtual function in the characterbase class that the playerclass and the enemy class can override.
		// This should play effects and Destroy enemies but have some kind of menu popup for player death with options to
		// restart from last save etc etc
	}
}

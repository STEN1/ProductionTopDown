// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


#include "NiagaraFunctionLibrary.h"
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

void UHealthComponent::SetDefaultHealth(float NewHealth)
{
	DefaultHealth = NewHealth;
	Health = DefaultHealth;
	HealthPercentage = Health / DefaultHealth;
	if (GameModeRef)
	{
		GameModeRef->UpdateHealthUI(Health, DefaultHealth);
	}
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
		TakingDamage();
	}
	if (Health <= 0)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(GetOwner());
		if(Character)Character->TriggerDeath();

		// check if the damaged actor is not a character
		if (!DamagedActor->IsA(ACharacterBase::StaticClass()))
		{
			SpawnDeathParticle();
			DamagedActor->Destroy();
		}
		// probably just call some kind of "HandleDeath()" function on the damaged actor.
		// we need to have a virtual function in the characterbase class that the playerclass and the enemy class can override.
		// This should play effects and Destroy enemies but have some kind of menu popup for player death with options to
		// restart from last save etc etc
	}
}

void UHealthComponent::SpawnDeathParticle()
{
	const FVector EffectSpawnLocationVector = GetOwner()->GetActorLocation();
	if (ActorDeathParticle && !ActorDeathParticle->IsLooping())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActorDeathParticle, EffectSpawnLocationVector);
	}
	if (ActorDeathParticle && ActorDeathParticle->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping particle emitter from %s"), *GetOwner()->GetHumanReadableName());
	}
	if (ActorDeathNiagaraParticle && !ActorDeathNiagaraParticle->IsLooping())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ActorDeathNiagaraParticle, EffectSpawnLocationVector);
	}
	if (ActorDeathNiagaraParticle && ActorDeathNiagaraParticle->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping niagra particle emitter from %s"), *GetOwner()->GetHumanReadableName());
	}
}

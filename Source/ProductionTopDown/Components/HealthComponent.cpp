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
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	SpawnHitParticle();
	if (Health <= 0)
	{
		if (bDead)
		{
			return;
		}
		bDead = true;
		
		SpawnActorOnDeath();
		SpawnDeathParticle();
		
		if (DeathSound)
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
			
		ACharacterBase* Character = Cast<ACharacterBase>(GetOwner());
		if(Character)Character->TriggerDeath();

		// check if the damaged actor is not a character
		if (!DamagedActor->IsA(ACharacterBase::StaticClass()))
		{
			DamagedActor->Destroy();
		}
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

void UHealthComponent::SpawnHitParticle()
{
	const FVector EffectSpawnLocationVector = GetOwner()->GetActorLocation();
	if (ActorHitParticle && !ActorHitParticle->IsLooping())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActorHitParticle, EffectSpawnLocationVector);
	}
	if (ActorHitParticle && ActorHitParticle->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping particle emitter from %s"), *GetOwner()->GetHumanReadableName());
	}
	if (ActorHitNiagaraParticle && !ActorHitNiagaraParticle->IsLooping())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ActorHitNiagaraParticle, EffectSpawnLocationVector);
	}
	if (ActorHitNiagaraParticle && ActorHitNiagaraParticle->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping niagra particle emitter from %s"), *GetOwner()->GetHumanReadableName());
	}
}

void UHealthComponent::SpawnActorOnDeath()
{
	if (ActorToSpawn)
	{
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	}
}

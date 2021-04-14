// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Components/HealthComponent.h"
#include "ProductionTopDown/Components/StaminaComponent.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	// StaminaComponent = CreateDefaultSubobject<UStaminaComponent>("Stamina Component");
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent = FindComponentByClass<UHealthComponent>();
	StaminaComponent = FindComponentByClass<UStaminaComponent>();

	if(GetMesh())GetMesh()->SetSimulatePhysics(false);
	
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

void ACharacterBase::TriggerDeath()
{
	if (DeathParticleNiagra && !DeathParticleNiagra->IsLooping())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathParticleNiagra, GetActorLocation());	
	}
	
	if(DeathParticle)UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	if(DeathSound)UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
	SpawnDeathParticle();


    //Ragdoll
    FTimerHandle Handle;
    	GetWorld()->GetTimerManager().SetTimer(Handle, [this]() {
            //code who runs after delay time
    		if(GetMesh())GetMesh()->SetSimulatePhysics(true);
        }, 0.1f, 0);
	
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

		//play particle
		SpawnAttackParticle();
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


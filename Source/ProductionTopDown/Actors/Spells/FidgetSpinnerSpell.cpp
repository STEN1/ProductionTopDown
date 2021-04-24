// Fill out your copyright notice in the Description page of Project Settings.


#include "FidgetSpinnerSpell.h"



#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProductionTopDown/Character/CharacterBase.h"


AFidgetSpinnerSpell::AFidgetSpinnerSpell()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 10.f;
}


void AFidgetSpinnerSpell::BeginPlay()
{
	Super::BeginPlay();

	PlayStartEffects();
	FireProsjectile();
}

void AFidgetSpinnerSpell::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ParticleSystemComponent)
		ParticleSystemComponent->Deactivate();
	if (NiagaraComponent)
		NiagaraComponent->Deactivate();

	GetWorldTimerManager().ClearTimer(FireProjectileHandle);
}


void AFidgetSpinnerSpell::PlayStartEffects()
{
	FVector SystemScale{1.f * CenterSystemScale, 1.f * CenterSystemScale, 1.f};
	if (CenterParticleSystem)
		ParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(CenterParticleSystem, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, SystemScale, EAttachLocation::KeepRelativeOffset, false);
	if (CenterNiagaraParticleSystem)
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(CenterNiagaraParticleSystem, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, SystemScale, EAttachLocation::KeepRelativeOffset, false, ENCPoolMethod::None);

	
}

void AFidgetSpinnerSpell::FireProsjectile()
{
	AActor* ProjectileOwner;

	if (GetOwner() && GetOwner()->IsA(ACharacterBase::StaticClass()))
		ProjectileOwner = GetOwner();
	else
		ProjectileOwner = this;
	
	GetWorldTimerManager().SetTimer(FireProjectileHandle, [this, ProjectileOwner]()
	{
		FVector SpawnLocation{GetActorLocation()};
		FRotator SpawnRotation{GetActorRotation()};
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = ProjectileOwner;
		GetWorld()->SpawnActor<AActor>(Projectile, SpawnLocation, SpawnRotation, SpawnParameters);
	}, FireCooldown, true);
}


void AFidgetSpinnerSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalRotation({0.f, SpinnerSpeed*DeltaTime, 0.f});
}


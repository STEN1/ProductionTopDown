// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpell.h"


#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ProductionTopDown/Character/CharacterBase.h"

AProjectileSpell::AProjectileSpell()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 15.f;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	HitBox = CreateDefaultSubobject<UBoxComponent>("HurtBox");
	HitBox->SetupAttachment(RootComponent);
	AoeHitBox = CreateDefaultSubobject<USphereComponent>("AoeHurtBox");
	AoeHitBox->SetupAttachment(RootComponent);
}


void AProjectileSpell::BeginPlay()
{
	Super::BeginPlay();
		SetActorEnableCollision(true);

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (HitBox)
	{
		
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectileSpell::OnHitBoxOverlap);
	}

	if (!SoundAttenuation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant play sound on: %s, no sound attenuation."), *GetHumanReadableName())
	}

	PlayLaunchEffects();
}

void AProjectileSpell::PlayLaunchEffects()
{
	FRotator LaunchRotation{GetActorRotation()};
	LaunchRotation.Yaw -= 90;
	if (LaunchNiagaraParticle && !LaunchNiagaraParticle->IsLooping())
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LaunchNiagaraParticle, GetActorLocation(), LaunchRotation, {0.5f,0.5f,0.5f});
	if (LaunchParticle && !LaunchParticle->IsLooping())
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchParticle, GetActorLocation(), GetActorRotation());

	if (FlyingNiagaraParticle)
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(FlyingNiagaraParticle, SceneComponent, {}, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);
	if (FlyingParticle)
		ParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(FlyingParticle, SceneComponent, {}, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1), EAttachLocation::KeepRelativeOffset, false);
	
	if (LaunchSound && SoundAttenuation)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchSound, GetActorLocation(), GetActorRotation(), 0.5f, 1, 0, SoundAttenuation);
}

void AProjectileSpell::PlayHitEffects()
{
	if (NiagaraComponent)
		NiagaraComponent->Deactivate();
	if (ParticleSystemComponent)
		ParticleSystemComponent->Deactivate();
	
	if (HitNiagaraParticle && !HitNiagaraParticle->IsLooping())
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagaraParticle, GetActorLocation(), GetActorRotation());
	if (HitParticle && !HitParticle->IsLooping())
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), GetActorRotation());
	
	if (HitSound && SoundAttenuation)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation(), GetActorRotation(), 0.5f, 1, 0, SoundAttenuation);
}

void AProjectileSpell::Destroyed()
{
	Super::Destroyed();
	PlayHitEffects();
}

void AProjectileSpell::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() && OtherActor->IsA(ACharacterBase::StaticClass())) return;
	
	if (OtherActor->IsA(ACharacterBase::StaticClass()) && OtherComp->IsA(UCapsuleComponent::StaticClass())
		|| !OtherActor->IsA(ACharacterBase::StaticClass()) && OtherComp->IsA(UStaticMeshComponent::StaticClass()))
	{
		if (AoeDamage)
		{
			TArray<UPrimitiveComponent*> Components;
			AoeHitBox->GetOverlappingComponents(Components);
			for (auto Component : Components)
			{
				if (Component->IsA(UCapsuleComponent::StaticClass()) && Component->GetOwner()->IsA(ACharacterBase::StaticClass())
				|| Component->IsA(UStaticMeshComponent::StaticClass()) && !Component->GetOwner()->IsA(ACharacterBase::StaticClass()))
				{
					UGameplayStatics::ApplyDamage(Component->GetOwner(), SpellDamage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
				}
				
			}
		}
		else
		{
			UGameplayStatics::ApplyDamage(OtherActor, SpellDamage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
		}
		
		Destroy();
	}

}



void AProjectileSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	AddActorLocalOffset({ MoveSpeed * DeltaTime, 0.f, 0.f }, true);
}


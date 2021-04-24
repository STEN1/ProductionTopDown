// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProjectileSpell.h"
#include "GameFramework/Actor.h"
#include "FidgetSpinnerSpell.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API AFidgetSpinnerSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	AFidgetSpinnerSpell();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* CenterParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* CenterNiagaraParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileSpell> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float CenterSystemScale{2.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float SpinnerSpeed{300.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float FireCooldown{0.2};
	
	void PlayStartEffects();
	void FireProsjectile();

	FTimerHandle FireProjectileHandle;

	class UNiagaraComponent* NiagaraComponent{ nullptr };
	UParticleSystemComponent* ParticleSystemComponent{ nullptr };
};

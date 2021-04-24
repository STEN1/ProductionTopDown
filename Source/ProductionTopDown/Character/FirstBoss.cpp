// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstBoss.h"
#include "BossAiController.h"
#include "GameFramework/CharacterMovementComponent.h"

AFirstBoss::AFirstBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
}

void AFirstBoss::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	if(WeaponMesh) WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
	
	GetCharacterMovement()->MaxWalkSpeed = 400;

	EnemyState = EBossState::Moving;
}

void AFirstBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	WalkSpeed = GetVelocity().Size();
	
}

EBossState AFirstBoss::GetEnemyState() const
{
	return EnemyState;
}

void AFirstBoss::SetEnemyState(EBossState State)
{
	EnemyState = State;
}

float AFirstBoss::GetWalkSpeed() const
{
	return WalkSpeed;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstBoss.h"
#include "BossAiController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "ProductionTopDown/Components/HealthComponent.h"
#include "kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.h"
#include "ProductionTopDown/Actors/Spells/ProjectileSpell.h"
#include "ProductionTopDown/Actors/Spells/FidgetSpinnerSpell.h"

AFirstBoss::AFirstBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackRange = CreateDefaultSubobject<UBoxComponent>("Attack Range Component");
	AttackRange->SetupAttachment(CharacterMesh, TEXT("AttackRange"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
}

void AFirstBoss::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	if(WeaponMesh) WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
	if(AttackRange) AttackRange->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("AttackRange"));
	if(AttackRange) AttackRange->SetRelativeScale3D(FVector(0.5f,0.5f,2.f));
	if(AttackRange) AttackRange->OnComponentBeginOverlap.AddDynamic(this, &AFirstBoss::OnComponentBeginOverlap);
	    
	GetCharacterMovement()->MaxWalkSpeed = 400;

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	BossState = EBossState::Moving;
}

void AFirstBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//stop doing anything if player is dead
	
	
	WalkSpeed = GetVelocity().Size();

	//UE_LOG(LogTemp, Warning, TEXT("Boss State %i"), GetEnemyState());
	
}

void AFirstBoss::TriggerDeath()
{
	Super::TriggerDeath();
	
	RemoveHealthbarEvent();

	this->Destroy();
}

EBossState AFirstBoss::GetEnemyState() const
{
	return BossState;
}

void AFirstBoss::SetEnemyState(EBossState State)
{
	BossState = State;
}

float AFirstBoss::GetWalkSpeed() const
{
	return WalkSpeed;
}

float AFirstBoss::GetAttackCounter()
{
	return AttackCounter;
}

void AFirstBoss::SetAttackCounter(float Counter)
{
	AttackCounter = Counter;
}

void AFirstBoss::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("BeginOverlapCalled"));
	if(OtherActor != this)
	{
		if(OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherComp->GetOwner()->IsA(ACharacterBase::StaticClass())
			|| !OtherComp->GetOwner()->IsA(ACharacterBase::StaticClass()))
		{
			{
				UGameplayStatics::ApplyDamage(
                    OtherComp->GetOwner(),
                    MeleeDamage,
                    GetInstigatorController(),
                    this,
                    DamageType
                    );
				//UE_LOG(LogTemp, Warning, TEXT("Trying to apply damage"));
			}
			const FVector PushBackVector = (OtherComp->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();	
			ACharacterBase* Characterbaseptr = Cast<ACharacterBase>(OtherComp->GetOwner());
			if(Characterbaseptr)Characterbaseptr->LaunchCharacter(PushBackVector*400.f, true, false);
				
		
		}
	}
}

void AFirstBoss::ToogleAttackRangeOverlap(bool EnableOverlap)
{
	if(AttackRange) AttackRange->SetGenerateOverlapEvents(EnableOverlap);	
}

void AFirstBoss::ShootFireBall()
{
	if(PlayerCharacter)
	{		
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector BossLocation = GetActorLocation();
		
		FVector SpawnLocation = BossLocation;
		SpawnLocation.Z += 150;
		FRotator SpawnRotation = (PlayerLocation - SpawnLocation).Rotation();
		
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(FireBallSpell, SpawnLocation, SpawnRotation, SpawnParameters);
	}
	//UE_LOG(LogTemp, Error, TEXT("Projectile Shot"));
	
}

void AFirstBoss::FidgetSpinAttack()
{
	FVector BossLocation = GetActorLocation();
	BossLocation.Z += 100;
	const FRotator BossRotation = GetActorRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	AActor* FidgetSpinner = GetWorld()->SpawnActor<AActor>(FidgetSpinSpell, BossLocation, BossRotation, SpawnParameters);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAiBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

AEnemyAiBase::AEnemyAiBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackRangeComponent = CreateDefaultSubobject<UBoxComponent>("Attack Range Component");
	AttackRangeComponent->SetupAttachment(CharacterMesh, TEXT("AttackRange"));

	WeaponMeshInHand = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
}

void AEnemyAiBase::BeginPlay()
{
	Super::BeginPlay();

	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	
	//600 is players walk speed. edit on each under class of enemy
	GetCharacterMovement()->MaxWalkSpeed = 600;

	if(AttackRangeComponent) AttackRangeComponent->SetRelativeScale3D(FVector(2.0f,2.0f,2.f));
	if(AttackRangeComponent) AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAiBase::OnComponentBeginOverlap);

	
	EnemyAiState = EEnemyAiState::Moving;
	
}

void AEnemyAiBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	WalkSpeed = GetVelocity().Size();
}

void AEnemyAiBase::TriggerDeath()
{
	Super::TriggerDeath();
	
	RemoveHealthbarEvent();

	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, [this]() {
		//code who runs after delay time
		this->Destroy();
	}, 30.f, 0);
}

void AEnemyAiBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Damage Code
	if(OtherActor != this)
    	{
    		if(OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherComp->GetOwner()->IsA(ACharacterBase::StaticClass())
    			|| !OtherComp->GetOwner()->IsA(ACharacterBase::StaticClass()))
    		{
    			{
    				UGameplayStatics::ApplyDamage(
                        OtherComp->GetOwner(),
                        20.f,
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

void AEnemyAiBase::ToogleAttackRangeOverlap(bool EnableOverlap)
{
	if(AttackRangeComponent) AttackRangeComponent->SetGenerateOverlapEvents(EnableOverlap);
}

void AEnemyAiBase::MeleeAttack()
{
	EnemyAiState = EEnemyAiState::MeleeAttack;
}

void AEnemyAiBase::SetEnemyState(EEnemyAiState NewState)
{
	EnemyAiState = NewState;
}

void AEnemyAiBase::Cleartimers()
{
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
}

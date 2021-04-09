// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "ProductionTopDown/Actors/Patrol/PatrolHub.h"
#include "ProductionTopDown/Actors/Spawning/Spawner.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Actors/Patrol/PatrolPoint.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"
#include "ProductionTopDown/Components/ScentComponent.h"

AEnemyBase::AEnemyBase()
{
	DetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
	DetectionComponent->SetupAttachment(RootComponent);
	AddInstanceComponent(DetectionComponent);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy %f, could not find the player!"), *GetOwner()->GetName());
		SetActorTickEnabled(false);
	} else
	{
		ScentComponent = Cast<UScentComponent>(Player->GetComponentByClass(UScentComponent::StaticClass()));
	}

	DetectionComponent->SetSphereRadius(DetectionRadius);
	DetectionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnComponentBeginOverlap);

	// if (!PatrolHub && Cast<ASpawner>(GetOwner()))
	// {
	// 	PatrolHub = Cast<ASpawner>(GetOwner())->PatrolHub;
	// }
	
	if (PatrolHub)
	{
		EnemyState = EEnemyState::Patrol;
	} else
	{
		EnemyState = EEnemyState::Idle;
	}
	
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayerClose)
	{
		IsPlayerInView();
	}
	
	switch (EnemyState)
	{
		case EEnemyState::Idle:
			IdleState(DeltaTime);
			break;
		case EEnemyState::Patrol:
			if (PatrolHub)
			{
				PatrolState();
			}
			break;
		case EEnemyState::Chase:
			FollowPlayer();
		break;
		case EEnemyState::Attack:
			AttackTimer += DeltaTime;
			if (AttackTimer >= AttackLenght)
			{
				EnemyState = EEnemyState::Chase;
				AttackTimer = 0.f;
			}
			Attack();
			break;
		default:
			break;
	}
	
}

void AEnemyBase::FollowPlayer()
{
	FVector MoveDir = GetMoveDirFromScent();
	
	if (MoveDir != FVector::ZeroVector)
	{
		SetActorRotation(MoveDir.Rotation());
	} else
	{
		EnemyState = EEnemyState::Idle;
	}
	
	MoveDir += GetMoveOffsetFromWall(100.f, ECC_Visibility);
	Move(0.5f, MoveDir);
}

FVector AEnemyBase::GetMoveDirFromScent()
{
	FHitResult Hit;
    Hit = GetFirstHitInReach(ECollisionChannel::ECC_Visibility, Player->GetActorLocation(), true);

	if (!Hit.IsValidBlockingHit())
	{
		FVector MoveDir = CalcVectorFromPlayerToTarget(Player->GetActorLocation()).GetSafeNormal2D();
		if (MoveDir != FVector::ZeroVector)
		{
			return MoveDir;
		}	
	}
	
	for (int i = ScentComponent->ScentArray.Num()-1; i >= 0 ; --i)
	{
		Hit = GetFirstHitInReach(ECollisionChannel::ECC_Visibility, ScentComponent->ScentArray[i], true);
		
		if (!Hit.IsValidBlockingHit())
        {
			FVector MoveDir = CalcVectorFromPlayerToTarget(ScentComponent->ScentArray[i]).GetSafeNormal2D();
        	if (MoveDir != FVector::ZeroVector)
        	{
        		return MoveDir;
        	}	
        }
	}
	return FVector::ZeroVector;
}

FVector AEnemyBase::GetMoveOffsetFromWall(float InReach, ECollisionChannel CollisionChannel)
{
	TArray<FHitResult> HitArray;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);

	for (int i = 1; i < 9; ++i)
	{
		FHitResult Hit;
			FRotator DirRot{0.f, i * 45.f,0.f};
        	FVector Dir = DirRot.Vector() * InReach;
		
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + Dir, CollisionChannel, TraceParams);
        	if (Hit.IsValidBlockingHit())
        	{
					HitArray.Add(Hit);
        		
        			DrawDebugLine(
                    GetWorld(),
                    GetActorLocation(),
                    Hit.Location,
                    FColor::Blue,
                    false,
                    0.f,
                    0,
                    5.f
                    );
        	} 
	}

	if (HitArray.Num() != 0)
	{
		FHitResult ReturnHit = HitArray[0];
		
		for (int i = 0; i < HitArray.Num(); ++i)
        {
			if (HitArray[i].Distance < ReturnHit.Distance)
	        {
				ReturnHit = HitArray[i];
			}
        		
        }
		return (CalcVectorFromPlayerToTarget(ReturnHit.Location) * -1);
	}

	return FVector::ZeroVector;;
}


FVector AEnemyBase::CalcVectorFromPlayerToTarget(FVector Target)
{
	FVector DirVector = Target - GetActorLocation();
	DirVector.Normalize(1.f);
	
	return DirVector;
}

void AEnemyBase::Move(float ScaleSpeed, FVector MoveDir)
{
	MoveDir = MoveDir.GetSafeNormal2D();
	AddMovementInput(MoveDir, ScaleSpeed);
}


FHitResult AEnemyBase::GetFirstHitInReach(ECollisionChannel CollisionChannel, FVector LineTraceEnd, bool DrawTraceLine) const
{
	FVector EnemyLocation{GetActorLocation()};
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByChannel(
        Hit,
        EnemyLocation,
        LineTraceEnd,
        CollisionChannel,
        TraceParams
    );

	// if (DrawTraceLine && Hit.IsValidBlockingHit())	//Draw to Hit out of sight
	// {
	// 	DrawDebugLine(
 //        GetWorld(),
 //        PawnLocation,
 //        Hit.Location,
 //        FColor::Red,
 //        false,
 //        0.f,
 //        0,
 //        5.f
	// 	);
	// } else
	if (DrawTraceLine && !Hit.IsValidBlockingHit())	//Draw to Hit in sight
	{
		DrawDebugLine(
		GetWorld(),
		EnemyLocation,
		LineTraceEnd,
		FColor::Blue,
		false,
		0.f,
		0,
		5.f
		);
	}
		return Hit;
}


void AEnemyBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacterBase::StaticClass()) && OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		bIsPlayerClose = true;
	}
}

void AEnemyBase::IsPlayerInView()
 {
	FHitResult Hit{GetFirstHitInReach(ECollisionChannel::ECC_Visibility, Player->GetActorLocation(), true)};

	if (!Hit.IsValidBlockingHit() && EnemyState != EEnemyState::Chase && EnemyState != EEnemyState::Attack)
	{
		EnemyState = EEnemyState::Chase;
	}

	if (FMath::Abs((GetActorLocation() - Player->GetActorLocation()).Size()) <= AttackRange)
	{
		EnemyState = EEnemyState::Attack;
	}
 }

void AEnemyBase::PatrolState()
{
	if (!bPatrolSet)
	{
		bool ValidPosFound{false};
		PatrolPointSelected = FVector::ZeroVector;
		FHitResult Hit;
		FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);
		for (int i = PatrolIndex; i < PatrolHub->PatrolPoints.Num(); ++i)
		{
			GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), PatrolHub->PatrolPoints[i]->GetActorLocation(), ECC_Visibility, TraceParams);

			DrawDebugLine(GetWorld(), GetActorLocation(), PatrolHub->PatrolPoints[i]->GetActorLocation(),FColor::Red);
			if (!Hit.IsValidBlockingHit())
			{
				PatrolPointSelected = PatrolHub->PatrolPoints[i]->GetActorLocation();
				PatrolIndex = i + 1;
				ValidPosFound = true;
			}else if (ValidPosFound)
			{
				bPatrolSet = true;
				return;
			}
		}
		PatrolIndex = 0;
		bPatrolSet = true;
		
	} else
	{
		FVector MoveDir = CalcVectorFromPlayerToTarget(PatrolPointSelected);
		MoveDir.Z = 0.f;
		Move(0.5f, MoveDir);
		if (MoveDir != FVector::ZeroVector)
		{
			SetActorRotation(MoveDir.Rotation());
		}
		
		if (FMath::Abs((GetActorLocation() - PatrolPointSelected).Size()) <= 100.f)
		{
			bPatrolSet = false;
			if (PatrolIndex == 0 && PatrolHub->PatrolPoints[PatrolHub->PatrolPoints.Num()-1]->bIsIdlePoint || PatrolIndex >= 1 && PatrolHub->PatrolPoints[PatrolIndex-1]->bIsIdlePoint)
			{
				EnemyState = EEnemyState::Idle;
			}
		
		}
	}
}

bool AEnemyBase::Attack()
{
	
	return true;
}

void AEnemyBase::IdleState(float DeltaTime)
{
	IdleTimer += DeltaTime;
	if (IdleTimer >= IdleTime)
	{
		IdleTimer = 0.f;
		PatrolIndex = 0;
        bPatrolSet = false;
		EnemyState = EEnemyState::Patrol;
	}
	if (FMath::Rand() % 40 == 1)
	{
		FVector RandDir{(float)(FMath::Rand() % 100), (float)(FMath::Rand() % 100), 0.f };
       	SetActorRotation(RandDir.Rotation());
	}
}

void AEnemyBase::TriggerDeath()
{
	Super::TriggerDeath();
	Destroy();
	
}

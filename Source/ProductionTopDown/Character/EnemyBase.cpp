// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"
#include "ProductionTopDown/Components/ScentComponent.h"

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
}


void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowPlayer();
	
}

FVector AEnemyBase::GetMoveDirFromScent()
{
	FHitResult Hit;
    Hit = GetFirstHitInReach(ECollisionChannel::ECC_WorldStatic, Player->GetActorLocation(), true);

	if (!Hit.IsValidBlockingHit())
	{
		FVector MoveDir = CalcVector(Player->GetActorLocation()).GetSafeNormal2D();
		if (MoveDir != FVector::ZeroVector)
		{
			return MoveDir;
		}	
	}
	
	for (int i = ScentComponent->ScentArray.Num()-1; i >= 0 ; --i)
	{
		Hit = GetFirstHitInReach(ECollisionChannel::ECC_WorldStatic, ScentComponent->ScentArray[i], true);
		
		if (!Hit.IsValidBlockingHit())
        {
			FVector MoveDir = CalcVector(ScentComponent->ScentArray[i]).GetSafeNormal2D();
        	if (MoveDir != FVector::ZeroVector)
        	{
        		return MoveDir;
        	}	
        }
	}


	return FVector::ZeroVector;
}

bool AEnemyBase::Attack()
{
	return true;
}

FVector AEnemyBase::CalcVector(FVector Target)
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

void AEnemyBase::FollowPlayer()
{
	FVector MoveDir = GetMoveDirFromScent();
	
	Move(0.5f, MoveDir);
	if (MoveDir != FVector::ZeroVector)
	{
		SetActorRotation(MoveDir.Rotation());
	}
}

FHitResult AEnemyBase::GetFirstHitInReach(ECollisionChannel CollisionChannel, FVector LineTraceEnd,
                                          bool DrawTraceLine) const
{
	FVector PawnLocation{GetActorLocation()};
	FRotator PawnRotation{GetActorRotation()};
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);
	GetWorld()->LineTraceSingleByObjectType(
        Hit,
        PawnLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(CollisionChannel),
        TraceParams
    );

	if (DrawTraceLine && Hit.IsValidBlockingHit())
	{
		DrawDebugLine(
        GetWorld(),
        PawnLocation,
        Hit.Location,
        FColor::Red,
        false,
        0.f,
        0,
        5.f
		);
	} else if (DrawTraceLine && !Hit.IsValidBlockingHit())
	{
		DrawDebugLine(
		GetWorld(),
		PawnLocation,
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


void AEnemyBase::TriggerDeath()
{
	Super::TriggerDeath();
	Destroy();
	
}

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

void AEnemyBase::FollowPlayer()
{
	FVector MoveDir = GetMoveDirFromScent();
	
	if (MoveDir != FVector::ZeroVector)
	{
		SetActorRotation(MoveDir.Rotation());
	}
	
	MoveDir += GetMoveOffsetFromWall(100.f, ECC_WorldStatic);
	Move(0.5f, MoveDir);
}

FVector AEnemyBase::GetMoveDirFromScent()
{
	FHitResult Hit;
    Hit = GetFirstHitInReach(ECollisionChannel::ECC_WorldStatic, Player->GetActorLocation(), true);

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
		Hit = GetFirstHitInReach(ECollisionChannel::ECC_WorldStatic, ScentComponent->ScentArray[i], true);
		
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
		
			GetWorld()->LineTraceSingleByObjectType(Hit, GetActorLocation(), GetActorLocation() + Dir, CollisionChannel, TraceParams);
        
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

bool AEnemyBase::Attack()
{
	return true;
}

void AEnemyBase::TriggerDeath()
{
	Super::TriggerDeath();
	Destroy();
	
}

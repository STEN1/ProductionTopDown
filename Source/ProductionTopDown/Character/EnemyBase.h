// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class APlayerCharacter;
class UScentComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle	= 0			UMETA(DisplayName = "Idle"),
    Patrol = 1		UMETA(DisplayName = "Following Patrol"),
    Chase = 2		UMETA(DisplayName = "Chasing Target"),
	Attack = 3		UMETA(DisplayName = "Attacking"),
};

UCLASS()
class PRODUCTIONTOPDOWN_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void TriggerDeath() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float Reach{500.f};

protected:
	virtual void BeginPlay() override;
	
	void Move(float ScaleSpeed, FVector MoveDir);
	void IsPlayerInView();
	void FollowPlayer();
	bool Attack();
	void IdleState();
	void PatrolState();

	
	FVector GetMoveDirFromScent();
	FVector GetMoveOffsetFromWall(float InReach, ECollisionChannel CollisionChannel);
	FVector CalcVectorFromPlayerToTarget(FVector Target);
	FHitResult GetFirstHitInReach(ECollisionChannel CollisionChannel, FVector LineTraceEnd, bool DrawTraceLine) const;
	
	APlayerCharacter* Player{nullptr};
	UScentComponent* ScentComponent{nullptr};
	class USphereComponent* DetectionComponent{nullptr};

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    float MoveSpeed{50.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float DetectionRadius{500.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float AttackRange{150.f};

	bool bIsPlayerClose{false};
	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState;
	
private:
	
};

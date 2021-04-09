// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle	= 0			UMETA(DisplayName = "Idle..."),
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
	void IdleState(float DeltaTime);
	void PatrolState();

	FVector GetMoveDirFromScent();
	FVector GetMoveOffsetFromWall(float InReach, ECollisionChannel CollisionChannel);
	FVector CalcVectorFromPlayerToTarget(FVector Target);
	FHitResult GetFirstHitInReach(ECollisionChannel CollisionChannel, FVector LineTraceEnd, bool DrawTraceLine) const;
	
	class APlayerCharacter* Player{nullptr};
	class UScentComponent* ScentComponent{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",  meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionComponent{nullptr};
	UPROPERTY(EditInstanceOnly)
	class APatrolHub* PatrolHub{nullptr};

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    float MoveSpeed{50.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float DetectionRadius{500.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float AttackRange{150.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float AttackLenght{1.5f};
	float AttackTimer{0.f};

	bool bIsPlayerClose{false};
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bPatrolSet{false};
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float PatrolIndex{0};
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float IdleTime{2.f};
	float IdleTimer{0.f};
	FVector PatrolPointSelected{0.f, 0.f, 0.f};
	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState;
	
private:
	
};

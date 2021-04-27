// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle	= 0			UMETA(DisplayName = "Idle..."),
    Patrol = 1			UMETA(DisplayName = "Following Patrol"),
    Chase = 2			UMETA(DisplayName = "Chasing Target"),
	Attack = 3			UMETA(DisplayName = "Attacking"),
	Dead = 4			UMETA(DisplayName = "Dead"),
};

UCLASS()
class PRODUCTIONTOPDOWN_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void TriggerDeath() override;

	virtual void Tick(float DeltaTime) override;
	
	void InitializeEnemyFromSpawner();
	
	UPROPERTY(EditAnywhere)
	float Reach{500.f};

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveHealthBar();

	UFUNCTION(BlueprintCallable)
	EEnemyState GetEnemyState();

	UFUNCTION(BlueprintCallable)
	void ToggleAttackBox(bool ToggleAttack);
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Move(float ScaleSpeed, FVector MoveDir);
	void IsPlayerInView();
	void FollowPlayer();
	virtual bool Attack() override;
	void IdleState(float DeltaTime);
	void PatrolState();

	FTimerHandle AttackTimerHandle;
	FTimerHandle DeathTimerHandle;
	
	UPROPERTY()
	class AAIController* EnemyAIController{nullptr};

	FVector GetMoveDirFromScent();
	FVector GetMoveOffsetFromWall(float InReach, ECollisionChannel CollisionChannel);
	FVector CalcVectorFromPlayerToTarget(FVector Target);
	FHitResult GetFirstHitInReach(ECollisionChannel CollisionChannel, FVector LineTraceEnd, bool DrawTraceLine) const;

	UPROPERTY()
	class APlayerCharacter* Player{nullptr};
	UPROPERTY()
	class UCapsuleComponent* EnemyCapsuleComponent{nullptr};
	UPROPERTY()
	class UScentComponent* ScentComponent{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",  meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionComponent{nullptr};
	UPROPERTY()
	class UBoxComponent* AttackBox{nullptr};
	UPROPERTY(EditInstanceOnly)
	class APatrolHub* PatrolHub{nullptr};
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnComponentBeginOverlapAttackBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    float MoveSpeed{50.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float DetectionRadius{500.f};
	FVector PatrolPointSelected{0.f, 0.f, 0.f};
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    FVector AttackBoxScale{200.f, 100.f, 100.f};
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
   	FVector AttackBoxOffset{100.f, 0.f, 0.f,};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float AttackRange{150.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float AttackLenght{1.5f};
	float AttackTimer{0.f};
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float AttackDamage{10.f};
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float AttackKnockback{100.f};
	bool bAttacking{false};

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bUseNavMesh{false};

	UPROPERTY(EditAnywhere, Category="Particle Effects")
	class UNiagaraSystem* AttackParticle;

	
	bool bIsPlayerClose{false};
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bPatrolSet{false};
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float PatrolIndex{0};
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float IdleTime{2.f};
	float IdleTimer{0.f};
	
	UPROPERTY(VisibleAnywhere)
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponInHand;
	
private:
	
};

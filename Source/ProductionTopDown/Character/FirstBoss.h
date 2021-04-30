// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"

#include "FirstBoss.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle = 0		UMETA(DisplayName = "Idle State"),
    Moving = 1		UMETA(DisplayName = "Moving state"),
    ChargeAttack = 2 UMETA(DisplayName = "Charge Attack State"),
	RangeAttack = 3 UMETA(DisplayName = "Range Attack State"),
	SpinAttack = 4	UMETA(DisplayName = "Spin Attack State"),
	NormalAttack = 5 UMETA(DisplayName = "Normal Attack State"),
	BigClearRoomAttack = 6 UMETA(DisplayName = "Big Clear Room Attack State"),
	HalfCharged = 7 UMETA(DisplayName = "Half Charge State")
};


class UBoxComponent;
class APlayerCharacter;
class AProjectileSpell;
class AFidgetSpinnerSpell;

UCLASS()
class PRODUCTIONTOPDOWN_API AFirstBoss : public ACharacterBase
{
	GENERATED_BODY()
	AFirstBoss();

	public:
	
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	EBossState GetEnemyState() const;
	void SetEnemyState(EBossState);
	
	float GetWalkSpeed() const;
	
	UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
	void ToogleAttackRangeOverlap(bool EnableOverlap);
	
	void ShootFireBall();
	void FidgetSpinAttack();
	protected:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	EBossState BossState;
	
	private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	UMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float WalkSpeed;
	
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackRange;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackRange2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DamageType")
    TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float MeleeDamage{20};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileSpell> FireBallSpell;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFidgetSpinnerSpell> FidgetSpinSpell;

	
	APlayerCharacter* PlayerCharacter;

	
};

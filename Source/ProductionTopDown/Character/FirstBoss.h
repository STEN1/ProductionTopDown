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
	BigClearRoomAttack = 6 UMETA(DisplayName = "Big Clear Room Attack State")
	
};

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
	
	protected:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	EBossState EnemyState;
	
	private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	UMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float WalkSpeed;
	
	USkeletalMeshComponent* CharacterMesh;

	
	
};

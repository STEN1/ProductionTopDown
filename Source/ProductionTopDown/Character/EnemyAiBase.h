// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyAiBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyAiState : uint8
{
	Idle = 0		UMETA(DisplayName = "Idle State"),
	Moving = 1		UMETA(DisplayName = "Moving state"),
	RangeAttack = 2 UMETA(DisplayName = "Range Attack State"),
	MeleeAttack = 3 UMETA(DisplayName = "Normal Attack State"),
};

class AWeaponBase;
class UBoxComponent;

UCLASS()
class PRODUCTIONTOPDOWN_API AEnemyAiBase : public ACharacterBase
{
	GENERATED_BODY()

	AEnemyAiBase();


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void TriggerDeath() override;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
	void ToogleAttackRangeOverlap(bool EnableOverlap);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveHealthbarEvent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	EEnemyAiState EnemyAiState;

	void MeleeAttack();

	void SetEnemyState(EEnemyAiState NewState);
	
protected:

	void Cleartimers();
private:
	//different weapons to spawn with
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<AWeaponBase*> WeaponBases;
	AWeaponBase* WeaponInHand;
	UStaticMeshComponent* WeaponMeshInHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackRangeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DamageType")
	TSubclassOf<UDamageType> DamageType;

	USkeletalMeshComponent* CharacterMesh;

	
	//for animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float WalkSpeed;

	//timer Handles
	FTimerHandle DeathTimerHandle;
};

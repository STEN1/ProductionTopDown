// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "NiagaraSystem.h"
#include "ProductionTopDown/Actors/Puzzle/Pushable_ActorBase.h"

#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Attacking = 0	UMETA(DisplayName = "Attack state"),
    Dashing = 1		UMETA(DisplayName = "Dashing state"),
    Moving = 2		UMETA(DisplayName = "Moving state"),
	Pushing = 3		UMETA(DisplayName = "Pushing state"),
	Dragging = 4	UMETA(DisplayName = "Draging state"),
	Dead = 5		UMETA(DisplayName = "Death State"),
	Charge = 6		UMETA(DisplayName = "Charge State"),
	HeavyAttack = 7	UMETA(DisplayName = "HeavyAttackState"),
	Jumping = 8		UMETA(DisplayName = "Jumping State")
};

class UInventoryComponent;
class AWeaponBase;
class UInteractComponent;
class UBoxComponent;

UCLASS()
class PRODUCTIONTOPDOWN_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void TriggerDeath() override;
	
	UFUNCTION(BlueprintCallable)
    void ResetWalkSpeed();
	UFUNCTION(BlueprintImplementableEvent)
	void SetStateToFalse();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnDashParticle();
	UFUNCTION(BlueprintImplementableEvent)
	void BPTriggerDeath();
	UFUNCTION(BlueprintCallable)
	void SetPlayerState(EPlayerState inpPlayerState);
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
    EPlayerState GetPlayerState();

	UFUNCTION(BlueprintCallable)
	float GetDashTimer();
	UFUNCTION(BlueprintCallable)
	float GetAttackTimer();

	void OnInventoryChange();

	UPROPERTY(BlueprintReadOnly)
	float WalkSpeedFloat;


	void RotateCharToMouse();

	UFUNCTION(BlueprintCallable)
	void SetMovingState();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//virtual bool Attack() override;
	virtual bool Dash() override;

	AActor* GetActorToDamage();
	void StartAttackTimer();
	void StopAttackTimer();
	void CalcAttackType();
	void LightAttack();
	void HeavyAttack();
	void DoubleHeavyAttack();
	
	float GetAttackDamage();
	
	void AttackEvent();
	void DashEvent();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateCharacter();
	
	void EquipWeaponFromInv(UStaticMesh* EquipWeapon);
	void StartDrag();
	void StopDrag();
	bool CheckForPushableActor();
	bool AbleToDrag();
	APushable_ActorBase* GetPushableActor();
	void PushObject(APushable_ActorBase* PushableActor);
	void DragObject(APushable_ActorBase* PushableActor);

	APushable_ActorBase* GetActorInFront();


	//debug functions
	void LogPlayerState();
	
private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	USkeletalMeshComponent* CharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInteractComponent* InteractComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackRangeComponent;
	
	//sounds
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	USoundBase* DashSound;
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	USoundBase* LightAttackSound;
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	USoundBase* HeavyAttackSound;
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	USoundBase* AttackChargeTimerSound;
	
	UPROPERTY(EditAnywhere, Category="Sound Effects")
    TArray<USoundBase*> AttackSounds;
	
	//CameraShake
	UPROPERTY(EditAnywhere, Category="Camera Effects")
	TSubclassOf<UMatineeCameraShake> DashShake;
	
	//particles
	UPROPERTY(EditAnywhere, Category="Particle Effects")
	UNiagaraSystem* LightAttackParticle;
	UPROPERTY(EditAnywhere, Category="Particle Effects")
	UNiagaraSystem* HeavyAttackParticle;
	UPROPERTY(EditAnywhere, Category="Particle Effects")
	UNiagaraSystem* DashParticle;
	UPROPERTY(EditAnywhere, Category="Particle Effects")
	UNiagaraSystem* AttackChargeTimerParticle;
	
	//variables
	FVector LastDirection;
	
	//InputVector
	float ForwardFloat;
	float SideWaysFloat;
	FVector InputVector;
	FVector LastInput;
	
	FRotator LastRotation;
	FRotator NullRotation;
	
	bool bCanDash{true};
	bool bAttackActive{false};
	bool bHeavyAttack{false};

	float StartAttackTime;
	float StopAttackTime;
	
	APlayerController* CharacterController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DamageType")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;

	
	UPROPERTY(EditAnywhere, Category="Movement")
	float DashDistance{5000};
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxWalkSpeed{600};
	UPROPERTY(EditAnywhere, Category="Health")
	float DefaultHealth{200};
	UPROPERTY(EditAnywhere, Category="Movement")
	float PushDistance{5};
	UPROPERTY(EditAnywhere, Category="Movement")
	float PushRange{100};
	EPlayerState PlayerState;
	FCriticalSection PlayerStateCriticalSection;
	FCriticalSection LevelStreamingCriticalSection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DashTimer{0.1f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DashDelay{1.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AttackTimer{0.6f};
	
	UPROPERTY(EditAnywhere, Category="Debug", meta = (AllowPrivateAccess = "true"));
	bool bDrawAttackRangeBox{false};
	
	bool bCanPush{false};

	bool bCanAttackAgain{true};

	//Timer Handles

	
	FTimerHandle AttackChargeTimerHandle;
	FTimerHandle DashCooldownHandle;
	FTimerHandle DashTimerHandle;
	FTimerHandle LightOverLapEventHandle;
	FTimerHandle LightMovingHandle;
	FTimerHandle HeavyOverLapEventHandle;
	FTimerHandle HeavyOverLapEventHandle2;
	FTimerHandle HeavyMovingHandle;
	FTimerHandle HeavyParticle2;

	UFUNCTION()
	void OnLevelLoaded();

	void HandleLevelStreamLoading();
	TArray<ULevelStreaming*> StreamingLevels;
	int32 NumberOfStreamingLevels{0};
};

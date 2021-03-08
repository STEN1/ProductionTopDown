// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Attacking = 0		UMETA(DisplayName = "Attack state"),
    Dashing = 1		UMETA(DisplayName = "Dashing state"),
    Moving = 2		UMETA(DisplayName = "Moving state"),
};

class UInventoryComponent;
class AWeaponBase;
class UInteractComponent;

UCLASS()
class PRODUCTIONTOPDOWN_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
    void ResetWalkSpeed();
	UFUNCTION(BlueprintImplementableEvent)
	void SetStateToFalse();
	UFUNCTION(BlueprintCallable)
	void SetPlayerState(EPlayerState inpPlayerState);
	
	UFUNCTION(BlueprintCallable)
    EPlayerState GetPlayerState();

	UFUNCTION(BlueprintCallable)
	float GetDashTimer();
	UFUNCTION(BlueprintCallable)
	float GetAttackTimer();
	
protected:
	virtual void BeginPlay() override;

	virtual bool Attack() override;
	virtual bool Dash() override;
	void AttackEvent();
	void DashEvent();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateCharacter();
	void RotateCharToMouse();
	
	void EquipWeaponFromInv(UStaticMeshComponent* EquipWeapon);

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	//debug functions
	void LogPlayerState();
private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	USkeletalMeshComponent* CharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInteractComponent* InteractComponent;

	UPROPERTY(EditAnywhere, Category="Particle Effects")
	UParticleSystem* DashParticle;
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	USoundBase* DashSound;
	UPROPERTY(EditAnywhere, Category="Camera Effects")
	TSubclassOf<UMatineeCameraShake> DashShake;

	
	//variables
	FVector LastDirection;
	FRotator LastRotation;
	
	APlayerController* CharacterController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DamageType")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;

	
	UPROPERTY(EditAnywhere, Category="Movement")
	float DashDistance{100};
	float MaxWalkSpeed{400};

	EPlayerState PlayerState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DashTimer{0.1f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AttackTimer{0.6f};
};

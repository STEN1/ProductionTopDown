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
	UFUNCTION(BlueprintCallable)
	bool GetIsDashing();
	UFUNCTION(BlueprintCallable)
	void SetIsDashing(bool bIsDashing);
	UFUNCTION(BlueprintCallable)
    bool GetIsBlocking();
	UFUNCTION(BlueprintCallable)
    void SetIsBlocking(bool bIsBlocking);
	UFUNCTION(BlueprintCallable)
    bool GetIsAttacking();
	UFUNCTION(BlueprintCallable)
    void SetIsAttacking(bool bIsAttacking);
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerState(EPlayerState inpPlayerState);
	
	UFUNCTION(BlueprintCallable)
    EPlayerState GetPlayerState();
	
protected:
	virtual void BeginPlay() override;

	virtual bool Attack() override;
	virtual bool Dash() override;
	void AttackEvent();
	void DashEvent();
	void MoverForward(float Value);
	void MoveRight(float Value);
	void RotateCharacter(float Value);

	void EquipWeaponFromInv(UStaticMeshComponent* EquipWeapon);
	
private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	USkeletalMeshComponent* CharacterMesh;

	

	//variables
	FVector LastDirection;
	FRotator LastRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;

	
	UPROPERTY(EditAnywhere, Category="Movement")
	float DashDistance{200};
	float MaxWalkSpeed{400};
	bool IsDashing{false};
	bool IsAttacking{false};
	bool IsBlocking{false};

	EPlayerState PlayerState;
};

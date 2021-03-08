// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter_Mouse.generated.h"

UENUM(BlueprintType)
enum class EnumPlayerState : uint8
{
	Attacking = 0		UMETA(DisplayName = "Attack state"),
    Dashing = 1		UMETA(DisplayName = "Dashing state"),
    Moving = 2		UMETA(DisplayName = "Moving state"),
	Idle = 3		UMETA(DisplayName = "Idle State"),
};

class UInventoryComponent;
class AWeaponBase;
class UInteractComponent;

UCLASS()
class PRODUCTIONTOPDOWN_API APlayerCharacter_Mouse : public ACharacterBase
{
	GENERATED_BODY()

	public:
	APlayerCharacter_Mouse();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	EnumPlayerState GetPlayerState();
	
	protected:
	FVector GetCursorLocationVector();
	void MoveToLocation(FVector NewLocation);
	void MoveEvent();
	
	private:
	//components
	
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
	APlayerController* CharacterController;
	EnumPlayerState PlayerState;

	UPROPERTY(EditAnywhere, Category="Movement", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed{150.f};
};

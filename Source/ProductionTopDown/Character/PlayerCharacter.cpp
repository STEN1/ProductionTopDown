// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProductionTopDown/Components/InventoryComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	
}

bool APlayerCharacter::Attack()
{
	// returns false if there is not enough stamina
	if (!Super::Attack()) return false;
	// Attack code here

	
	return true;
}

bool APlayerCharacter::Dash()
{
	// returns false if there is not enough stamina
	if (!Super::Dash()) return false;
	// Dash code here

	IsDashing = true;
	
	//GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	
	//FVector DashVector;
	//DashVector.X = LastDirection.X;
	//DashVector.Y = LastDirection.Y;
	//DashVector.Z = 0;
	//DashVector = DashVector.GetSafeNormal(); // Normalize
	
	//LaunchCharacter(DashVector*DashDistance, true, true);

	//timer or something to set dashing false
	//IsDashing = false;
	return true;
}

void APlayerCharacter::AttackEvent()
{
	Attack();
}

void APlayerCharacter::DashEvent()
{
	//Dash Animation and particles
	
	Dash();
}

void APlayerCharacter::MoverForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void APlayerCharacter::RotateCharacter(float Value)
{
	
	
	float VLen = GetVelocity().Size();
	FRotator MeshRotation = GetVelocity().Rotation();
	MeshRotation.Yaw -= 90; //rotates the char så den blir rett vei
	MeshRotation.Pitch = 0;
	//SetActorRotation(ActorRotation);
	if (CharacterMesh && VLen != 0)
	{
		LastDirection = GetVelocity();
		LastRotation = MeshRotation;
		CharacterMesh->SetWorldRotation(MeshRotation);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsDashing)RotateCharacter(1);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackEvent);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::DashEvent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoverForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("MouseX",this, &APlayerCharacter::RotateCharacter);
}

bool APlayerCharacter::GetIsDashing()
{
	return IsDashing;
}

void APlayerCharacter::SetIsDashing(bool bIsDashing)
{
	IsDashing = bIsDashing;
}

bool APlayerCharacter::GetIsBlocking()
{
	return IsBlocking;
}

void APlayerCharacter::SetIsBlocking(bool bIsBlocking)
{
	IsBlocking = bIsBlocking;
}

bool APlayerCharacter::GetIsAttacking()
{
	return IsAttacking;
}

void APlayerCharacter::SetIsAttacking(bool bIsAttacking)
{
	IsAttacking  = bIsAttacking;
}

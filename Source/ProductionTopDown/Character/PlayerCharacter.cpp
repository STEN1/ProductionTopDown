// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
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

	Jump();
	return true;
}

void APlayerCharacter::AttackEvent()
{
	Attack();
}

void APlayerCharacter::DashEvent()
{
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
	FVector ChVelocity = GetVelocity();
	float VLen = ChVelocity.Size();
	FRotator MeshRotation = ChVelocity.Rotation();
	MeshRotation.Yaw -= 90; //rotates the char så den blir rett vei
	MeshRotation.Pitch = 0;
	//SetActorRotation(ActorRotation);
	if (CharacterMesh && VLen != 0)
	{
		CharacterMesh->SetWorldRotation(MeshRotation);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateCharacter(1);
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

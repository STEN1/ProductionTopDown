// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProductionTopDown/Components/InventoryComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(CharacterMesh, TEXT("WeaponSocket"));
	
}

EPlayerState APlayerCharacter::GetPlayerState()
{
	return PlayerState;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//start in moving state
	PlayerState = EPlayerState::Moving;
	LogPlayerState();
	
	CharacterMesh = FindComponentByClass<USkeletalMeshComponent>();
	
	//testing purposes
	if(Weapon)EquipWeaponFromInv(Weapon);
	
	
}

bool APlayerCharacter::Attack()
{
	// returns false if there is not enough stamina
	if (!Super::Attack()) return false;
	// Attack code here
	//makes you walk in half speed when attacking
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed()/2.f;
	
	//PlayerState = EPlayerState::Attacking;
	LogPlayerState();
	
	
	//attack delay
	const float DelayTime = 0.7f;
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
        //code who runs after delay time
        PlayerState = EPlayerState::Moving;
        LogPlayerState();
		ResetWalkSpeed();
    }, DelayTime, 0);
	return true;
}

bool APlayerCharacter::Dash()
{
	// returns false if there is not enough stamina
	if (!Super::Dash()) return false;
	// Dash code here

	//IsDashing = true;
	//PlayerState = EPlayerState::Dashing;
	LogPlayerState();
	
	FVector DashDirection  = LastDirection.GetSafeNormal()*DashDistance;
	DashDirection.Z = 0;
	LaunchCharacter(DashDirection, true , false);

	//delay until dash is finish
	const float DelayTime = 0.5f;
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
		//code who runs after delay time
		PlayerState = EPlayerState::Moving;
		LogPlayerState();
    }, DelayTime, 0);
	
	return true;
}

void APlayerCharacter::AttackEvent()
{
	if(InventoryComponent->GetItemObject()!= nullptr)
	{
		PlayerState = EPlayerState::Attacking;
		Attack();
	}
	
}

void APlayerCharacter::DashEvent()
{
	//Dash Animation and particles
	PlayerState = EPlayerState::Dashing;
	Dash();
}

void APlayerCharacter::MoveForward(float Value)
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

void APlayerCharacter::EquipWeaponFromInv(UStaticMeshComponent* EquipWeapon)
{
	//EquipWeapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
}

void APlayerCharacter::LogPlayerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Player state is : %i"), PlayerState);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerState != EPlayerState::Dashing)RotateCharacter(1);


	switch (PlayerState)
	{
	case EPlayerState::Attacking:
		//attack state
		break;
	case EPlayerState::Dashing:
		//dashing state
		break;
	case EPlayerState::Moving:
		//move state
		break;

	default:
		
		break;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackEvent);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::DashEvent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("MouseX",this, &APlayerCharacter::RotateCharacter);
}


void APlayerCharacter::SetPlayerState(EPlayerState inpPlayerState)
{
	PlayerState = inpPlayerState;
}

void APlayerCharacter::ResetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

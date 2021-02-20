// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

APlayerPawn::APlayerPawn()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}


// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate();
	Move(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this, &APlayerPawn::SetForwarInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::SetRightInput);
	PlayerInputComponent->BindAxis("MouseX", this, &APlayerPawn::SetMouseX);
}

void APlayerPawn::Move(float DeltaTime)
{
	FVector MoveV{ ForwardInput, RightInput, 0.f };
	AddActorLocalOffset(MoveV * DeltaTime * MoveSpeed, true);
}

void APlayerPawn::Rotate()
{
	FRotator RotationR{ 0.f, MouseX, 0.f };
	AddActorLocalRotation(RotationR * MouseSens, false);
}

void APlayerPawn::SetForwarInput(float Value)
{
	ForwardInput = Value;
}

void APlayerPawn::SetRightInput(float Value)
{
	RightInput = Value;
}

void APlayerPawn::SetMouseX(float Value)
{
	MouseX = Value;
}
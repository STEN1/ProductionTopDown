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


	Move();
	Rotate();
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this, &APlayerPawn::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Rotate", this, &APlayerPawn::CalculateRotateInput);
}

void APlayerPawn::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APlayerPawn::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}

void APlayerPawn::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value*MoveSpeed*GetWorld()->DeltaTimeSeconds,0 ,0 );
}

void APlayerPawn::CalculateRotateInput(float Value)
{
	float RotateAmount = Value*RotateSpeed*GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount ,0);
	RotationDirection = FQuat(Rotation);
}
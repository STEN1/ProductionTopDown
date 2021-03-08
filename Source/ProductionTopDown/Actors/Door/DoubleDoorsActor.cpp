// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleDoorsActor.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

ADoubleDoorsActor::ADoubleDoorsActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
	LeftDoor->SetupAttachment(RootComponent);
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
	RightDoor->SetupAttachment(RootComponent);
	
}

void ADoubleDoorsActor::BeginPlay()
{
	Super::BeginPlay();

	LeftStartRotation = LeftDoor->GetRelativeRotation();
	RightStartRotation = RightDoor->GetRelativeRotation();

	if (TriggerVolume)
	{
		TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &ADoubleDoorsActor::BeginOverlap);
        TriggerVolume->OnActorEndOverlap.AddDynamic(this, &ADoubleDoorsActor::EndOverlap);
	}

	TargetRotation.Yaw = TargetYaw;

	SetActorTickEnabled(false);
}

void ADoubleDoorsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpen || bStayOpen)
	{
		FRotator NewRotation = TargetRotation;
		NewRotation = FMath::InterpExpoOut(LeftDoor->GetRelativeRotation(), TargetRotation, OpenSpeed * DeltaTime);
		LeftDoor->SetRelativeRotation(NewRotation);
		RightDoor->SetRelativeRotation(NewRotation * (-1.f));

		UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), NewRotation.Yaw);
		if (NewRotation.Yaw >= TargetRotation.Yaw -0.1f)
		{
			SetActorTickEnabled(false);
		}
	}
	else
	{
		CloseTimer += DeltaTime;
		if (CloseTimer >= CloseDelay && !bStayOpen)
		{
			FRotator NewRotation = LeftStartRotation;
            NewRotation = FMath::InterpExpoOut(LeftDoor->GetRelativeRotation(), LeftStartRotation, OpenSpeed * DeltaTime);
            LeftDoor->SetRelativeRotation(NewRotation);
            RightDoor->SetRelativeRotation(NewRotation * (-1.f));
            
            if (NewRotation.Yaw == LeftStartRotation.Yaw -0.1f)
            {
            	SetActorTickEnabled(false);
            }
		} else if (bStayOpen)
		{
			SetActorTickEnabled(false);
		}

	}
}

void ADoubleDoorsActor::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		SetActorTickEnabled(true);
		bIsOpen = !bIsOpen;
		UE_LOG(LogTemp, Warning, TEXT("BeginOverlap!"))
	}
}

void ADoubleDoorsActor::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<APlayerCharacter>(OtherActor))
	{
		SetActorTickEnabled(true);
		bIsOpen = !bIsOpen;
	}
}

void ADoubleDoorsActor::OpenFromInteract()
{
	SetActorTickEnabled(true);
	bIsOpen = !bIsOpen;
}
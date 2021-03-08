// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter_Mouse.h"

#include <activation.h>


#include "GameFramework/CharacterMovementComponent.h"
#include "ProductionTopDown/Components/InventoryComponent.h"
#include "kismet/GameplayStatics.h"
#include "ProductionTopDown/Components/InteractComponent.h"

#include "GameFramework/PlayerController.h"

#include "Components/SkeletalMeshComponent.h"


APlayerCharacter_Mouse::APlayerCharacter_Mouse()
{
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");

}

void APlayerCharacter_Mouse::BeginPlay()
{
	PlayerState = EnumPlayerState::Idle;

	
	//show cursor on begin play
	CharacterController = GetWorld()->GetFirstPlayerController();
	if (CharacterController)
	{
		CharacterController->bShowMouseCursor = true;
	}
}

void APlayerCharacter_Mouse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LMousebutton", IE_Pressed, this , &APlayerCharacter_Mouse::MoveEvent);
}

void APlayerCharacter_Mouse::Tick(float DeltaTime)
{
	if(PlayerState == EnumPlayerState::Moving)
	{
		
	}
	
}

EnumPlayerState APlayerCharacter_Mouse::GetPlayerState()
{
	return PlayerState;
}

FVector APlayerCharacter_Mouse::GetCursorLocationVector()
{
	//variables
	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	
	//move actor to cursor
	CharacterController->GetHitResultUnderCursorByChannel(TraceChannel,true, HitResult);
	const FVector NewLocation = HitResult.Location;
	
	return NewLocation;
}

void APlayerCharacter_Mouse::MoveToLocation(FVector NewLocation)
{
	FVector CurrentLocation;
	
	NewLocation.Z = GetActorLocation().Z;

	SetActorLocation(NewLocation);
	if (CurrentLocation.X == NewLocation.X && CurrentLocation.Y == NewLocation.Y)
	{
		PlayerState = EnumPlayerState::Idle;
	}
}


void APlayerCharacter_Mouse::MoveEvent()
{
	PlayerState = EnumPlayerState::Moving;
	//MoveToLocation(GetActorLocation(),GetCursorLocationVector());
}

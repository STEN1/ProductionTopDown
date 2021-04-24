// Fill out your copyright notice in the Description page of Project Settings.


#include "RustySword.h"

#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

ARustySword::ARustySword()
{
	ThisItemClass = RustySword;
	bIsWeapon = true;
}

bool ARustySword::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
	if (PlayerCharacter && World)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter,
                                    1.f,
                                    PlayerCharacter->GetInstigatorController(),
                                    PlayerCharacter,
                                    UDamageType::StaticClass()
                                    );

		WorldPtr = World;
		
		if (PlayerCharacter->GetPlayerState() != EPlayerState::Moving) return true;
		PlayerCharacter->SetPlayerState(EPlayerState::Attacking);
		PlayerCharacter->RotateCharToMouse();
		WorldPtr->GetTimerManager().SetTimer(UseCooldownHandle, [PlayerCharacter]()
		{
			PlayerCharacter->SetPlayerState(EPlayerState::Moving);
		}, UseCooldown, false);
		
		FVector SpawnLocation{PlayerCharacter->GetActorLocation()};
		FRotator SpawnRotation{PlayerCharacter->FindComponentByClass<USkeletalMeshComponent>()->GetComponentRotation()};
		SpawnRotation.Yaw += 90.f;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = PlayerCharacter;
		SpawnParameters.Instigator = PlayerCharacter;
		AActor* Projectile = World->SpawnActor<AActor>(ProjectileSpell, SpawnLocation, SpawnRotation, SpawnParameters);
		return true;
	}
	return false;
}

void ARustySword::BeginPlay()
{
	Super::BeginPlay();
}

void ARustySword::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (WorldPtr)
		WorldPtr->GetTimerManager().ClearTimer(UseCooldownHandle);
}

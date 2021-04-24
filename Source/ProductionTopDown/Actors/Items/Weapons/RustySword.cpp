// Fill out your copyright notice in the Description page of Project Settings.


#include "RustySword.h"

#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

ARustySword::ARustySword()
{
	ThisItemClass = RustySword;
	bIsWeapon = true;
}

void ARustySword::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
	if (PlayerCharacter && World)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter,
                                    1.f,
                                    PlayerCharacter->GetInstigatorController(),
                                    PlayerCharacter,
                                    UDamageType::StaticClass()
                                    );
		FVector SpawnLocation{PlayerCharacter->GetActorLocation()};
		FRotator SpawnRotation{PlayerCharacter->FindComponentByClass<USkeletalMeshComponent>()->GetComponentRotation()};
		SpawnRotation.Yaw += 90.f;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = PlayerCharacter;
		AActor* Projectile = World->SpawnActor<AActor>(ProjectileSpell, SpawnLocation, SpawnRotation, SpawnParameters);
		if (Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("FIREBALL"));
		}
	}
}

void ARustySword::BeginPlay()
{
	Super::BeginPlay();
}

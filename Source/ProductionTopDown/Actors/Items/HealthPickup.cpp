// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

AHealthPickup::AHealthPickup()
{
	ItemName = TEXT("Health Pot");
	ItemLore = TEXT("Even better than modern medicine.");
	ThisItemClass = HealthPickup;
	bConsumable = true;
}

bool AHealthPickup::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
	if (PlayerCharacter && World)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter,
                                    -20.f,
                                    World->GetFirstPlayerController(),
                                    PlayerCharacter,
                                    UDamageType::StaticClass()
                                    );
		UE_LOG(LogTemp, Warning, TEXT("Calling use on HealthPickup."));
		return true;
	}
	
	return false;
}


void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

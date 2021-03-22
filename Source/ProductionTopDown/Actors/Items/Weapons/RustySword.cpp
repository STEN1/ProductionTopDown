// Fill out your copyright notice in the Description page of Project Settings.


#include "RustySword.h"

#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

ARustySword::ARustySword()
{
	ItemName = TEXT("Rusty Sword");
	ItemLore = TEXT("A rusty piece of steel, forged by a blacksmiths apprentice.");
	ThisItemClass = RustySword;
	bIsWeapon = true;
}

void ARustySword::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
	if (PlayerCharacter && World)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter,
                                    10.f,
                                    PlayerCharacter->GetInstigatorController(),
                                    PlayerCharacter,
                                    UDamageType::StaticClass()
                                    );
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Took DMG trying to use rusty sword."));
}

void ARustySword::BeginPlay()
{
	Super::BeginPlay();
}

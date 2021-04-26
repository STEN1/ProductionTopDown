// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	bIsWeapon = true;
}

bool AWeaponBase::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
	return false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

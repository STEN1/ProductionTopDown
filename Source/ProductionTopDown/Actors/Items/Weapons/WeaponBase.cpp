// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	bIsWeapon = true;
}

void AWeaponBase::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

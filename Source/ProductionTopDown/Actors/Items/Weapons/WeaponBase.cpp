// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	bPickup = true;
	bUsable = false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

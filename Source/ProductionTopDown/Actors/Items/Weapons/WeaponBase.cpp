// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	bPickup = true;
	bUsable = false;
	
}

float AWeaponBase::GetMinDamage() const
{
	return MinDamage;
}

float AWeaponBase::GetMaxDamage() const
{
	return MaxDamage;
}

float AWeaponBase::GetCritChance() const
{
	return CritChance;
}

float AWeaponBase::GetKnockbackAmount() const
{
	return KnockbackAmount;
}



void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

AHealthPickup::AHealthPickup()
{
	ItemName = TEXT("Health Pot");
	ItemLore = TEXT("Even better than modern medicine.");
	ThisItemClass = HealthPickup;
}

void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

AHealthPickup::AHealthPickup()
{
	ItemName = TEXT("Health Pot");
	ItemLore = TEXT("Even better than modern medicine.");
	ThisItemClass = HealthPickup;
	bConsumable = true;
}

void AHealthPickup::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling use on HealthPickup."));
}


void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

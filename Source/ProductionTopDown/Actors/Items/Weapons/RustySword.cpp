// Fill out your copyright notice in the Description page of Project Settings.


#include "RustySword.h"

ARustySword::ARustySword()
{
	ItemName = TEXT("Rusty Sword");
	ItemLore = TEXT("A rusty piece of steel, forged by a blacksmiths apprentice.");
	ThisItemClass = RustySword;
}

void ARustySword::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling use on Rusty Sword."));
}

void ARustySword::BeginPlay()
{
	Super::BeginPlay();
}

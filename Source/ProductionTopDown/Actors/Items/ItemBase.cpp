// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "ProductionTopDown/Character/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/RustySword.h"


AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	BoxComponent->SetupAttachment(RootComponent);
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(RootComponent);
	
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	WidgetComponent->SetVisibility(false, true);
	OnActorBeginOverlap.AddDynamic(this, &AItemBase::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AItemBase::EndOverlap);
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	BoxComponent->SetGenerateOverlapEvents(true);
	WidgetComponent->SetGenerateOverlapEvents(false);
	
}

void AItemBase::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s should show now.."), *ItemName);
		if (GetRootComponent()->IsVisible())
		{
			WidgetComponent->SetVisibility(true, true);
		}
	}
}

void AItemBase::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s should hide now.."), *ItemName);
		WidgetComponent->SetVisibility(false, true);
	}
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AItemBase::GetItemName() const
{
	return ItemName;
}

FString AItemBase::GetItemLore() const
{
	return ItemLore;
}

float AItemBase::GetMinDamage() const
{
	return MinDamage;
}

float AItemBase::GetMaxDamage() const
{
	return MaxDamage;
}

float AItemBase::GetCritChance() const
{
	return CritChance;
}

float AItemBase::GetKnockbackAmount() const
{
	return KnockbackAmount;
}

ItemClass AItemBase::GetItemClass() const
{
	return ThisItemClass;
}

UTexture2D* AItemBase::GetItemImage() const
{
	if (ItemImage)
	{
		return ItemImage;
	}
	return nullptr;
}

void AItemBase::UseItem(APlayerCharacter* PlayerCharacter, UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling use on AItemBase."));
}


bool AItemBase::IsConsumable()
{
	return bConsumable;
}

bool AItemBase::IsWeapon()
{
	return bIsWeapon;
}

void AItemBase::SpawnItemOfItemClass(ItemClass Item, AActor* Actor, FVector Location, FRotator Rotation)
{
	{

		// Cant spawn BP classes this way.. and if i do, i also have to chose to pack the BP files
		// With the project to make it work on a final build.. PITA
		
		switch (Item)
		{
		case Empty:
			UE_LOG(LogTemp, Warning, TEXT("Cant throw nothing.. Are you drunk?"));
			break;
		case RustySword:
			Actor->GetWorld()->SpawnActor<ARustySword>(ARustySword::StaticClass(), Location, Rotation);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Missing spawning implementation of item in ItemBase.cpp -> SpawnItemOfItemClass()"));
			break;
		}
	}
}




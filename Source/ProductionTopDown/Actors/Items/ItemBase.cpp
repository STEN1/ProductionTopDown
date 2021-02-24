// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "ProductionTopDown/Character/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"


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
	
}

void AItemBase::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s should show now.."), *ItemName);
		WidgetComponent->SetVisibility(true, true);
	}
}

void AItemBase::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s should hide now.."), *ItemName);
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

ItemClass AItemBase::GetItemClass() const
{
	return ThisItemClass;
}


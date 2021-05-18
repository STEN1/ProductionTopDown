// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogInteractable.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

ADialogInteractable::ADialogInteractable()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComponent;
}

void ADialogInteractable::Interact(bool Condition)
{
}

void ADialogInteractable::BeginPlay()
{
	Super::BeginPlay();
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADialogInteractable::BeginOverlap);
		BoxComponent->SetGenerateOverlapEvents(true);
	}
	
}

void ADialogInteractable::BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		PlayerOverlaps();
		BoxComponent->SetGenerateOverlapEvents(false);
	}
}


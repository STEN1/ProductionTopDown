// Fill out your copyright notice in the Description page of Project Settings.


#include "TextScroll.h"


#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ProductionTopDown/ProductionTopDownGameModeBase.h"

#include "Kismet/GameplayStatics.h"

ATextScroll::ATextScroll()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(RootComponent);

}

void ATextScroll::Interact(bool Condition)
{
	if (GameModeRef)
	{
		GameModeRef->TextBoxString = ScrollText;
		GameModeRef->DisplayTextBox();
	}
}

void ATextScroll::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<AProductionTopDownGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (Mesh && BoxComponent && WidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("beginplay"))
		WidgetComponent->SetVisibility(false, true);
		OnActorBeginOverlap.AddDynamic(this, &ATextScroll::BeginOverlap);
		OnActorEndOverlap.AddDynamic(this, &ATextScroll::EndOverlap);
	}


	
}

void ATextScroll::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("AASdasdasd"))
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (WidgetComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("AASdasdasd"))
			WidgetComponent->SetVisibility(true, false);
		}
	}
}

void ATextScroll::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("AASdasdasd"))
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (WidgetComponent)
		{
			WidgetComponent->SetVisibility(false, false);
		}
	}
}


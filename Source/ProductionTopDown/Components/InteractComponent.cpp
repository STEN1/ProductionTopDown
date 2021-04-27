// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/MyGameInstance.h"
#include "ProductionTopDown/Actors/Interactables/InteractableBase.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Box"));
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (bDrawDebugBox)
	{
		DrawDebugBox(GetWorld(),BoxComponent->GetComponentLocation(), BoxComponent->GetScaledBoxExtent(), BoxComponent->GetComponentRotation().Quaternion() ,FColor::Red,false,0.f, 0,10);
	}
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CreateBoxComponent();
	
	PlayerInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!PlayerInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No input component on: %s"), *GetOwner()->GetHumanReadableName());
	}
	else
	{
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &UInteractComponent::Interact);
	}
	
	
}

void UInteractComponent::Interact()
{
	TArray<AActor*> ActorArray;
	TSubclassOf<AInteractableBase> Base = AInteractableBase::StaticClass();
	BoxComponent->GetOverlappingActors(ActorArray, Base);


	if (ActorArray.Num() > 0)
	{
		Cast<AInteractableBase>(ActorArray.Last())->Interact();
	}

}

void UInteractComponent::CreateBoxComponent()
{
	USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	BoxComponent = NewObject<UBoxComponent>(GetOwner(), UBoxComponent::StaticClass());

	if (BoxComponent && Mesh)
	{
		BoxComponent->SetupAttachment(Mesh);
		BoxComponent->RegisterComponent();
		BoxComponent->SetRelativeLocation({0.f, 0.f, 50.f});
		BoxComponent->SetRelativeScale3D({5, 5, 3.f});
	}

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UHurtBoxComponent::UHurtBoxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HurtBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HurtBox"));
}


// Called when the game starts
void UHurtBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	HurtBox = NewObject<UBoxComponent>(GetOwner(), UBoxComponent::StaticClass());
	HurtBox->OnComponentBeginOverlap.AddDynamic(this, &UHurtBoxComponent::BeginOverlap);
	HurtBox->OnComponentEndOverlap.AddDynamic(this, &UHurtBoxComponent::EndOverlap);
	
	MeshAttachedTo = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	
	if (MeshAttachedTo && HurtBox)
	{
		HurtBox->SetupAttachment(MeshAttachedTo);
		HurtBox->SetRelativeScale3D(BoxScale);
		HurtBox->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HurtBoxComponent on Actor %s, is NOT attached!"), *GetOwner()->GetHumanReadableName());
	}
	UE_LOG(LogTemp, Error, TEXT("HurtBoxComponent attached to: %s"), *MeshAttachedTo->GetName());
}

// Called every frame
void UHurtBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HurtBox->SetRelativeLocation(MeshAttachedTo->GetRelativeLocation());

	DrawDebugBox(GetWorld(),HurtBox->GetComponentLocation(), HurtBox->GetScaledBoxExtent(), HurtBox->GetComponentRotation().Quaternion() ,FColor::Red,false,0.f, 0,10);
}

void UHurtBoxComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerController>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("HURT BOX HIT"));
        //UGameplayStatics::ApplyDamage(OtherActor, Damage, OtherActor->GetInstigatorController(), GetOwner()->GetInstigatorController(), DamageTypes);
	}

}

void UHurtBoxComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

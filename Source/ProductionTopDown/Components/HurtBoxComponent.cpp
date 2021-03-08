// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UHurtBoxComponent::UHurtBoxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HurtBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HurtBoxCollisionBox"));
	HurtBox->RegisterComponent();
}


// Called when the game starts
void UHurtBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	HurtBox->OnComponentBeginOverlap.AddDynamic(this, &UHurtBoxComponent::BeginOverlap);
	HurtBox->OnComponentEndOverlap.AddDynamic(this, &UHurtBoxComponent::EndOverlap);
	HurtBox->SetupAttachment(GetOwner()->FindComponentByClass<USceneComponent>());
	
}

// Called every frame
void UHurtBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHurtBoxComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, OtherActor->GetInstigatorController(), GetOwner()->GetInstigatorController(), DamageTypes);
}

void UHurtBoxComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

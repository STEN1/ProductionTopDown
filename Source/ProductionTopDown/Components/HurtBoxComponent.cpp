// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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
	//HurtBox->OnComponentEndOverlap.AddDynamic(this, &UHurtBoxComponent::EndOverlap);
	
	MeshAttachedTo = GetOwner()->FindComponentByClass<UStaticMeshComponent>();	//Will attach to the first Static mesh component in the hierarchy
	
	if (MeshAttachedTo && HurtBox)
	{
		HurtBox->SetupAttachment(MeshAttachedTo);
		HurtBox->SetRelativeScale3D(BoxScale);
		HurtBox->SetRelativeLocation(MeshAttachedTo->GetRelativeLocation() + BoxOffset);
		HurtBox->RegisterComponent();
		HurtBox->SetCollisionObjectType(ECC_GameTraceChannel2);

		SetComponentTickEnabled(bDrawDebugBox);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HurtBoxComponent on Actor %s, is NOT attached!"), *GetOwner()->GetHumanReadableName());
	}
	
}

// Called every frame
void UHurtBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bDrawDebugBox)
	{
		DrawDebugBox(GetWorld(),HurtBox->GetComponentLocation(), HurtBox->GetScaledBoxExtent(), HurtBox->GetComponentRotation().Quaternion() ,FColor::Red,false,0.f, 0,10);
	}
}

void UHurtBoxComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor) && Cast<UCapsuleComponent>(OtherComp))
	{
        UGameplayStatics::ApplyDamage(OtherActor, Damage, OtherActor->GetInstigatorController(), GetOwner()->GetInstigatorController(), UDamageType::StaticClass());

		FVector PushBackVector = (OtherComp->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal2D();
		APlayerCharacter* PlayerCharacterTemp = Cast<APlayerCharacter>(OtherComp->GetOwner());
		if (PlayerCharacterTemp)
			PlayerCharacterTemp->LaunchCharacter(PushBackVector*DamageKnockback + KnockbackJump, true, false);
	}
}

// void UHurtBoxComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// }

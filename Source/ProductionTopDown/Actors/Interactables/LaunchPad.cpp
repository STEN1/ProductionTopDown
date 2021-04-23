// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/CharacterBase.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

ALaunchPad::ALaunchPad()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	ActivateBox = CreateDefaultSubobject<UBoxComponent>("ActivateBox");
	ActivateBox->SetupAttachment(RootComponent);
}

void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();

	if (ActivateBox)
	{
		ActivateBox->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::ActivateBeginOverlap);
	}
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorTickEnabled(false);
}

void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StaticMeshComponent->AddRelativeLocation({0.f, 0.f, AnimationSpeed*DeltaTime});

	if (StaticMeshComponent->GetRelativeLocation().Z > PadAnimationZDistance)
	{
		StaticMeshComponent->SetRelativeLocation({0.f, 0.f, PadAnimationZDistance});
		AnimationSpeed = -AnimationSpeed;
		PlaySwooshSound();
	}
	else if (StaticMeshComponent->GetRelativeLocation().Z < 0.f)
	{
		StaticMeshComponent->SetRelativeLocation({0.f, 0.f, 0.f});
		AnimationSpeed = -AnimationSpeed;
		SetActorTickEnabled(false);
	}

}

void ALaunchPad::ActivateBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			PlayerCharacter->LaunchCharacter({0.f, 0.f, CharacterLaunchSpeed}, true, true);
			SetActorTickEnabled(true);
		}
	}
}

void ALaunchPad::PlaySwooshSound() const
{
	if (SwooshSound && SoundAttenuation)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwooshSound, GetActorLocation(), GetActorRotation(), 0.5f, 1 ,0, SoundAttenuation);
	}
}
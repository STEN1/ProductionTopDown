// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpikes.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/CharacterBase.h"

AWallSpikes::AWallSpikes()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	HurtBox = CreateDefaultSubobject<UBoxComponent>("HurtBox");
	HurtBox->SetupAttachment(StaticMeshComponent);
	ActivateBox = CreateDefaultSubobject<UBoxComponent>("ActivateBox");
	ActivateBox->SetupAttachment(RootComponent);
	
}

void AWallSpikes::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	StaticMeshComponent->AddRelativeLocation({0.f, CurrentSpeed*DeltaSeconds, 0.f});
	
	if (StaticMeshComponent->GetRelativeLocation().Y > LaunchDistance)
	{
		StaticMeshComponent->SetRelativeLocation({0.f, LaunchDistance, 0.f});
		CurrentSpeed = -RetractSpeed;
	}
	else if (StaticMeshComponent->GetRelativeLocation().Y < 0.f)
	{
		StaticMeshComponent->SetRelativeLocation({0.f, 0.f, 0.f});
		CurrentSpeed = LaunchSpeed;
		if (bWantToStop || !bLooping)
		{
			bActive = false;
			SetActorTickEnabled(false);
		}
		else
		{
			bActive = false;
			SetActorTickEnabled(false);
			Activate(true);
		}
	}
	
}

void AWallSpikes::Activate(bool On)
{
	if (On)
	{
		if (bActive) return;
		
		CurrentSpeed = LaunchSpeed;
		bActive = true;
		if (StartDelay > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(StartTimerHandle, [this]()
			{
				bWantToStop = false;
				PlaySwooshSound();
				SetActorTickEnabled(true);
			}, StartDelay, false);
		}
		else
		{
			bWantToStop = false;
			PlaySwooshSound();
			SetActorTickEnabled(true);
		}
	}
	else
	{
		bWantToStop = true;
	}
}

void AWallSpikes::BeginPlay()
{
	Super::BeginPlay();

	if (HurtBox && ActivateBox)
	{
		HurtBox->OnComponentBeginOverlap.AddDynamic(this, &AWallSpikes::HurtBeginOverlap);
		ActivateBox->OnComponentBeginOverlap.AddDynamic(this, &AWallSpikes::ActivateBeginOverlap);
	}

	SetActorTickEnabled(false);

	if (bStartActive)
	{
		Activate(true);
	}
}

void AWallSpikes::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(StartTimerHandle);
}

void AWallSpikes::HurtBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacterBase* CharacterBase = Cast<ACharacterBase>(OtherActor))
	{
		if (OtherComp->IsA(UCapsuleComponent::StaticClass()))
		{
			FVector LaunchVector{(CharacterBase->GetActorLocation() - HurtBox->GetComponentLocation()).GetSafeNormal2D()};
			CharacterBase->LaunchCharacter(LaunchVector * KnockBackAmount, false, false);
			
			UGameplayStatics::ApplyDamage(CharacterBase,
											9999,
											GetInstigatorController(),
											this,
											UDamageType::StaticClass());
		}
	}
}

void AWallSpikes::ActivateBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		Activate(true);	
	}
}

void AWallSpikes::PlaySwooshSound() const
{
	if (SwooshSound && SoundAttenuation)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SwooshSound, GetActorLocation(), GetActorRotation(), 0.5f, 1 ,0, SoundAttenuation);
	}
}
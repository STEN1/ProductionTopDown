// Fill out your copyright notice in the Description page of Project Settings.


#include "Pendulum.h"



#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/CharacterBase.h"

APendulum::APendulum()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(RootComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
}

void APendulum::Activate(bool On)
{
	if (On)
	{
		if (StartDelay > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(StartTimerHandle, [this]()
			{
				bWantToStop = false;
				SetActorTickEnabled(true);
			}, StartDelay, false);
		}
		else
		{
			bWantToStop = false;
			SetActorTickEnabled(true);
		}
	}
	else
	{
		bWantToStop = true;
	}
}

void APendulum::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	StaticMeshComponent->AddRelativeRotation({RotationSpeed*DeltaSeconds, 0.f, 0.f}, true);

	
	if (StaticMeshComponent->GetRelativeRotation().Pitch < -80.f)
	{
		StaticMeshComponent->SetRelativeRotation({-80.f, 0.f, 0.f});
		RotationSpeed = -RotationSpeed;

		if (!bLoop || bWantToStop)
		{
			SetActorTickEnabled(false);
			return;
		}
		AddSwingDelay();
	}
	else if (StaticMeshComponent->GetRelativeRotation().Pitch > 80.f)
	{
		StaticMeshComponent->SetRelativeRotation({80.f, 0.f, 0.f});
		RotationSpeed = -RotationSpeed;

		if (!bLoop || bWantToStop)
		{
			SetActorTickEnabled(false);
			return;
		}
		AddSwingDelay();
	}
}

void APendulum::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &APendulum::OnHit);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APendulum::OnOverlap);
	
	SetActorTickEnabled(false);

	if (bStartActive)
	{
		Activate(true);
	}
}

void APendulum::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 9999.f, GetInstigatorController(), this, UDamageType::StaticClass());
	}
}

void APendulum::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		Activate(true);
	}
}

void APendulum::AddSwingDelay()
{
	if (DelayBetweenSwing > 0.f)
	{
		SetActorTickEnabled(false);
		GetWorld()->GetTimerManager().ClearTimer(StartTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(StartTimerHandle, [this]()
        {
            SetActorTickEnabled(true);
        }, DelayBetweenSwing, false);
	}
}



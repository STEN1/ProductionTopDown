// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Camera/CameraComponent.h"
#include "Components/BrushComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProductionTopDown/Character/PlayerCharacter.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	
	TargetLocation = FVector(
        StartLocation.X + TargetXPos,
        StartLocation.Y + TargetYPos,
        StartLocation.Z + TargetZPos);
	TargetRotation = FRotator(
        StartRotation.Pitch,
        StartRotation.Yaw + TargetYaw,
        StartRotation.Roll);

	if (OpenTrigger)
	{
		OpenTrigger->GetBrushComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::BeginOverlap);
		OpenTrigger->GetBrushComponent()->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::EndOverlap);
	}
	SetActorTickEnabled(false);
}

void ADoorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(CloseTimerHandle);
	AudioComponent->Stop();
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bDoorOpen)
	{
		switch(DoorTypeOpen)
		{
			case EDoorType::Constant:
				OpenDoor(DeltaTime);
				break;
			case EDoorType::EaseOut:
				EaseOpenDoor(DeltaTime);
				break;
			case EDoorType::Accelerate:
				AccelOpenDoor(DeltaTime);
				break;
		}
	}
	else
	{
		switch(DoorTypeClose)
		{
			case EDoorType::Constant:
				CloseDoor(DeltaTime);
				break;
			case EDoorType::EaseOut:
				EaseCloseDoor(DeltaTime);
				break;
			case EDoorType::Accelerate:
				AccelCloseDoor(DeltaTime);
				break;
		}
	}

}

void ADoorActor::Activate(bool On)
{
	if (CloseDelay > 0.f)
	{
		if (On)
		{
			GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
			bDoorOpen = On;
			ExpoSpeed = 10.f;
			SetActorTickEnabled(true);
			AudioComponent->Play();
		}
		else
		{
			if (bStayOpen)
			{
				return;
			}
			GetWorld()->GetTimerManager().ClearTimer(CloseTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(CloseTimerHandle, [this, On]()
			{
				bDoorOpen = On;
				ExpoSpeed = 10.f;
				SetActorTickEnabled(true);
				AudioComponent->Stop();
			}, CloseDelay, false);
		}
	}
	else
	{
		if (!On && bStayOpen)
		{
			return;
		}
		bDoorOpen = On;
		ExpoSpeed = 10.f;
		SetActorTickEnabled(true);
		if (On)
		{
			AudioComponent->Play();
		}
		else
		{
			AudioComponent->Stop();
		}
	}

}

void ADoorActor::SetAlwaysMoving(bool AlwaysMoving)
{
	bAlwaysMoving = AlwaysMoving;
}

void ADoorActor::OpenDoor(float DeltaTime)
{
	FVector NewLocation = TargetLocation;
	NewLocation.X = FMath::FInterpConstantTo(GetActorLocation().X, TargetLocation.X, DeltaTime, OpenSpeed);
	NewLocation.Y = FMath::FInterpConstantTo(GetActorLocation().Y, TargetLocation.Y, DeltaTime, OpenSpeed);
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, TargetLocation.Z, DeltaTime, OpenSpeed);
	SetActorLocation(NewLocation);

	CurrentYawOffset = FMath::FInterpConstantTo(CurrentYawOffset, TargetYaw, DeltaTime, OpenSpeed);
	SetActorRotation({StartRotation.Pitch, StartRotation.Yaw + CurrentYawOffset, StartRotation.Roll});

	if (CurrentYawOffset == TargetYaw
		&& NewLocation.X == TargetLocation.X
		&& NewLocation.Y == TargetLocation.Y
		&& NewLocation.Z == TargetLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Open! Constant."))
		
		if (bAlwaysMoving)
		{
			bDoorOpen = !bDoorOpen;
			ExpoSpeed = 10.f;
		}
		else
		{
			SetActorTickEnabled(false);
			OnDoorOpened();
		}
	}
}

void ADoorActor::CloseDoor(float DeltaTime)
{
	FVector NewLocation = StartLocation;
	NewLocation.X = FMath::FInterpConstantTo(GetActorLocation().X, StartLocation.X, DeltaTime, CloseSpeed);
	NewLocation.Y = FMath::FInterpConstantTo(GetActorLocation().Y, StartLocation.Y, DeltaTime, CloseSpeed);
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, StartLocation.Z, DeltaTime, CloseSpeed);
	SetActorLocation(NewLocation);

	CurrentYawOffset = FMath::FInterpConstantTo(CurrentYawOffset, 0.f, DeltaTime, OpenSpeed);
	SetActorRotation({StartRotation.Pitch, StartRotation.Yaw + CurrentYawOffset, StartRotation.Roll});

	if (CurrentYawOffset == 0.f
		&& NewLocation.X == StartLocation.X
		&& NewLocation.Y == StartLocation.Y
		&& NewLocation.Z == StartLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Closed! Constant."))
		if (bAlwaysMoving)
		{
			bDoorOpen = !bDoorOpen;
			ExpoSpeed = 10.f;
		}
		else
		{
			SetActorTickEnabled(false);
			OnDoorClosed();
		}
	}
}

void ADoorActor::EaseOpenDoor(float DeltaTime)
{
	FVector NewLocation = TargetLocation;
	NewLocation.X = FMath::InterpExpoOut(GetActorLocation().X, TargetLocation.X, DeltaTime * OpenSpeed * 0.005f);
	NewLocation.Y = FMath::InterpExpoOut(GetActorLocation().Y, TargetLocation.Y, DeltaTime * OpenSpeed * 0.005f);
	NewLocation.Z = FMath::InterpExpoOut(GetActorLocation().Z, TargetLocation.Z, DeltaTime * OpenSpeed * 0.005f);
	
	SetActorLocation(NewLocation);

	CurrentYawOffset = FMath::InterpExpoOut(CurrentYawOffset, TargetYaw, DeltaTime * OpenSpeed * 0.005f);
	SetActorRotation({StartRotation.Pitch, StartRotation.Yaw + CurrentYawOffset, StartRotation.Roll});

	// FMath::Abs(TargetLocation.X - NewLocation.X);
	if (FMath::Abs(TargetYaw - CurrentYawOffset) < 0.01f
		&& FMath::Abs(TargetLocation.X - NewLocation.X) < 0.01f
		&& FMath::Abs(TargetLocation.Y - NewLocation.Y) < 0.01f
		&& FMath::Abs(TargetLocation.Z - NewLocation.Z) < 0.01f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Open! Ease out."))
		if (bAlwaysMoving)
		{
			bDoorOpen = !bDoorOpen;
			ExpoSpeed = 10.f;
		}
		else
		{
			SetActorTickEnabled(false);
			OnDoorOpened();
		}
	}
}

void ADoorActor::EaseCloseDoor(float DeltaTime)
{
	FVector NewLocation = StartLocation;
	NewLocation.X = FMath::InterpExpoOut(GetActorLocation().X, StartLocation.X, DeltaTime * OpenSpeed * 0.005f);
	NewLocation.Y = FMath::InterpExpoOut(GetActorLocation().Y, StartLocation.Y, DeltaTime * OpenSpeed * 0.005f);
	NewLocation.Z = FMath::InterpExpoOut(GetActorLocation().Z, StartLocation.Z, DeltaTime * OpenSpeed * 0.005f);
	SetActorLocation(NewLocation);

	CurrentYawOffset = FMath::InterpExpoOut(CurrentYawOffset, 0.f, DeltaTime * OpenSpeed * 0.005f);
	SetActorRotation({StartRotation.Pitch, StartRotation.Yaw + CurrentYawOffset, StartRotation.Roll});

	if (FMath::Abs(CurrentYawOffset) < 0.01f
	&& FMath::Abs(StartLocation.X - NewLocation.X) < 0.01f
    && FMath::Abs(StartLocation.Y - NewLocation.Y) < 0.01f
    && FMath::Abs(StartLocation.Z - NewLocation.Z) < 0.01f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Closed! Ease out."))
		if (bAlwaysMoving)
		{
			bDoorOpen = !bDoorOpen;
			ExpoSpeed = 10.f;
		}
		else
		{
			SetActorTickEnabled(false);
			OnDoorClosed();
		}
	}
}

void ADoorActor::AccelOpenDoor(float DeltaTime)
{
	FVector NewLocation = StartLocation;
	NewLocation.X = FMath::FInterpConstantTo(GetActorLocation().X, TargetLocation.X, DeltaTime, OpenSpeed + ExpoSpeed);
	NewLocation.Y = FMath::FInterpConstantTo(GetActorLocation().Y, TargetLocation.Y, DeltaTime, OpenSpeed + ExpoSpeed);
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, TargetLocation.Z, DeltaTime, OpenSpeed + ExpoSpeed);
	SetActorLocation(NewLocation);

	CurrentYawOffset = FMath::FInterpConstantTo(CurrentYawOffset, TargetYaw, DeltaTime, OpenSpeed + ExpoSpeed);
	SetActorRotation({StartRotation.Pitch, StartRotation.Yaw + CurrentYawOffset, StartRotation.Roll});
	
	ExpoSpeed = FMath::Clamp(FMath::Pow(ExpoSpeed, 1.025f),0.f,MaxExpoSpeed);

	if (CurrentYawOffset == TargetYaw
		&& NewLocation.X == TargetLocation.X
		&& NewLocation.Y == TargetLocation.Y
		&& NewLocation.Z == TargetLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Open! Accel."))

		//Particle FX and Sound here***
		
		if (bAlwaysMoving)
		{
			bDoorOpen = !bDoorOpen;
			ExpoSpeed = 10.f;
		}
		else
		{
			SetActorTickEnabled(false);
			OnDoorOpened();
		}
	}

}

void ADoorActor::AccelCloseDoor(float DeltaTime)
{
	FVector NewLocation = TargetLocation;
	NewLocation.X = FMath::FInterpConstantTo(GetActorLocation().X, StartLocation.X, DeltaTime, OpenSpeed + ExpoSpeed);
	NewLocation.Y = FMath::FInterpConstantTo(GetActorLocation().Y, StartLocation.Y, DeltaTime, OpenSpeed + ExpoSpeed);
	NewLocation.Z = FMath::FInterpConstantTo(GetActorLocation().Z, StartLocation.Z, DeltaTime, OpenSpeed + ExpoSpeed);
	SetActorLocation(NewLocation);

	CurrentYawOffset = FMath::FInterpConstantTo(CurrentYawOffset, 0.f, DeltaTime, OpenSpeed + ExpoSpeed);
	SetActorRotation({StartRotation.Pitch, StartRotation.Yaw + CurrentYawOffset, StartRotation.Roll});

	ExpoSpeed = FMath::Clamp(FMath::Pow(ExpoSpeed, 1.025f),0.f,MaxExpoSpeed);
	// ExpoSpeed = FMath::Clamp(FMath::Pow(ExpoSpeed, 1.015f),0.f,MaxExpoSpeed);

	if (CurrentYawOffset == 0.f
		&& NewLocation.X == StartLocation.X
		&& NewLocation.Y == StartLocation.Y
		&& NewLocation.Z == StartLocation.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Closed! Accel."))

		//Particle FX and Sound here***
		
		if (bAlwaysMoving)
		{
			bDoorOpen = !bDoorOpen;
			ExpoSpeed = 10.f;
		}
		else
		{
			SetActorTickEnabled(false);
			OnDoorClosed();
		}
	}
}

void ADoorActor::OnDoorOpened()
{
	AudioComponent->Stop();
}

void ADoorActor::OnDoorClosed()
{
	if (ClosedSound)
	{
		if (ClosedParticle && !ClosedParticle->IsLooping())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ClosedParticle, GetActorLocation() + FVector{0.f, 0.f, 40.f}, GetActorRotation() + FRotator{0.f, 90.f, 0.f});
		}
		UGameplayStatics::PlaySoundAtLocation(this, ClosedSound, GetActorLocation(), GetActorRotation());
	}
}

void ADoorActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		Activate(true);
	}
}

void ADoorActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->IsA(UCapsuleComponent::StaticClass()) && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		Activate(false);
	}
}
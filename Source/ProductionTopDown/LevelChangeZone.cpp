// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChangeZone.h"



#include "MyGameInstance.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelChangeZone::ALevelChangeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ChangeZone = CreateDefaultSubobject<UBoxComponent>("ChangeZone");
	RootComponent = ChangeZone;

}

// Called when the game starts or when spawned
void ALevelChangeZone::BeginPlay()
{
	Super::BeginPlay();
	if (ChangeZone)
	{
		ChangeZone->OnComponentBeginOverlap.AddDynamic(this, &ALevelChangeZone::BeginOverlap);
	}
}

void ALevelChangeZone::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && NextLevel != "")
	{
		Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SaveCurrentInventoryToInstance();
		UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
	}
}



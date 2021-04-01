// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"



#include "MySaveGame.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SaveZone = CreateDefaultSubobject<UBoxComponent>("SaveZone");
	RootComponent = SaveZone;

}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();

	SaveZone->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnOverlap);
}

void ASavePoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		UMySaveGame::SaveGame(this, "Slot1");
	}
}




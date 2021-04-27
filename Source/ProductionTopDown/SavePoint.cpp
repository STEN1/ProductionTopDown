// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"



#include "MySaveGame.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

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

	TArray<AActor*> OverlappingActors;
	SaveZone->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
	for (auto OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->IsA(APlayerCharacter::StaticClass()))
		{
			if (!UMySaveGame::SaveGame(this, "Slot1", GetName()))
			{
				return;
			}
			SpawnSaveParticle(OverlappingActor->GetActorLocation());
			OnSaved();
		}
	}
}

void ASavePoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && OtherComp->IsA(UCapsuleComponent::StaticClass()))
	{
		if (!UMySaveGame::SaveGame(this, "Slot1", GetName()))
		{
			return;
		}
		SpawnSaveParticle(OtherActor->GetActorLocation());
		OnSaved();
	}
}

void ASavePoint::SpawnSaveParticle(FVector EffectSpawnLocationVector)
{
	EffectSpawnLocationVector.Z += 200.f;
	if (PSTemplate && !PSTemplate->IsLooping())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PSTemplate, EffectSpawnLocationVector);
	}
	if (PSTemplate && PSTemplate->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping particle emitter from %s"), *GetHumanReadableName());
	}
	if (NSTemplate && !NSTemplate->IsLooping())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSTemplate, EffectSpawnLocationVector);
	}
	if (NSTemplate && NSTemplate->IsLooping())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant spawn looping niagra particle emitter from %s"), *GetHumanReadableName());
	}
}




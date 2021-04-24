// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProductionTopDown/Actors/Items/ItemBase.h"
#include "ProductionTopDown/Actors/Spells/ProjectileSpell.h"

#include "RustySword.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API ARustySword : public AItemBase
{
	GENERATED_BODY()
public:
	ARustySword();

	virtual void UseItem(class APlayerCharacter* PlayerCharacter, UWorld* World) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileSpell> ProjectileSpell;
	
};

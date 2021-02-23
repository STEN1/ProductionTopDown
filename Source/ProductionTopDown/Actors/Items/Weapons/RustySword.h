// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RustySword.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API ARustySword : public AWeaponBase
{
	GENERATED_BODY()
public:
	ARustySword();
protected:
	virtual void BeginPlay() override;
	
};

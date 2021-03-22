// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "TextScroll.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API ATextScroll : public AInteractableBase
{
	GENERATED_BODY()
public:
	ATextScroll();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true", MultiLine = "true"))
	FString ScrollText{ "Shift\nPluss Enter\nTo make multiline text" };
};

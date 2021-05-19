// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "DialogInteractable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DialogData")
	UTexture2D* DialogImageLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DialogData")
	UTexture2D* DialogImageRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DialogData", meta = (MultiLine = "true"))
	FString DialogText;
	
};

UCLASS()
class PRODUCTIONTOPDOWN_API ADialogInteractable : public AInteractableBase
{
	GENERATED_BODY()

public:
	ADialogInteractable();
	void Interact(bool Condition = true) override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerOverlaps();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	TArray<FDialogData> DialogArray;
	
private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


};

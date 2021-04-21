// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "ProductionTopDown/Actors/Interactables/ActivatableBase.h"
#include "Pendulum.generated.h"

/**
 * 
 */
UCLASS()
class PRODUCTIONTOPDOWN_API APendulum : public AActivatableBase
{
	GENERATED_BODY()

public:
	APendulum();
	virtual void Activate(bool On) override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bStartActive{true};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float StartDelay{0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bLoop{true};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float DelayBetweenSwing{2.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float RotationSpeed{500.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	USoundBase* SwooshSound;

	UPROPERTY(EditAnywhere, Category="Sound")
	class USoundAttenuation* SoundAttenuation;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

private:
	FTimerHandle StartTimerHandle;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void AddSwingDelay();
	void PlaySwooshSound() const;

	bool bWantToStop{false};
};

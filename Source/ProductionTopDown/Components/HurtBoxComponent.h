// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HurtBoxComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRODUCTIONTOPDOWN_API UHurtBoxComponent : public UActorComponent
{
	GENERATED_BODY()

	class UBoxComponent* HurtBox{nullptr};

public:	
	// Sets default values for this component's properties
	UHurtBoxComponent();

	UPROPERTY(EditAnywhere, Category="HurtBox")
	float Damage{50.f};
	UPROPERTY(EditAnywhere, Category="HurtBox")
	float DamageKnockback{500};
	UPROPERTY(EditAnywhere, Category="HurtBox")
	FVector KnockbackJump{0.f, 0.f, 250.f};
	UPROPERTY(EditAnywhere, Category="HurtBox")
	FVector BoxScale{2.5f, 2.5f, 1.f};
	UPROPERTY(EditAnywhere, Category="HurtBox")
	FVector BoxOffset{0.f, 0.f, 10.f};
	UPROPERTY(VisibleAnywhere, Category="HurtBox")
	class UStaticMeshComponent* MeshAttachedTo{nullptr};
	UPROPERTY(EditAnywhere, Category="HurtBox")
	bool bDrawDebugBox{false};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	// UFUNCTION()
 //    void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

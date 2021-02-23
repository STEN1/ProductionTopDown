// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemBase.generated.h"

UCLASS()
class PRODUCTIONTOPDOWN_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();
	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	FString GetItemName() const;

protected:
	virtual void BeginPlay() override;
	bool bUsable{ true };
	bool bPickup{ false };

	FString ItemName{TEXT("")};
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;
	

	//
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};

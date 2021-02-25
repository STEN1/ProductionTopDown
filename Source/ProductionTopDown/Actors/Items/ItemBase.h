// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemBase.generated.h"
enum ItemClass
{
	Empty,
    RustySword,
	HealthPickup
};
UCLASS()
class PRODUCTIONTOPDOWN_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();
	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	FString GetItemName() const;
	UFUNCTION(BlueprintCallable)
	FString GetItemLore() const;
	
	ItemClass GetItemClass() const;
	UTexture2D* GetItemImage() const;

	static void SpawnItemOfItemClass(ItemClass Item, AActor* Actor, FVector Location, FRotator Rotation);

protected:
	virtual void BeginPlay() override;
	bool bUsable{ true };
	bool bPickup{ false };

	ItemClass ThisItemClass{ Empty };

	FString ItemName{TEXT("")};
	FString ItemLore{TEXT("")};
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings", meta = (AllowPrivateAccess = "true"))
	UTexture2D* ItemImage;
	
	//
	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};

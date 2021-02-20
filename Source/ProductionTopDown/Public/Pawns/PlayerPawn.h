// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/BasePawn.h"
#include "PlayerPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PRODUCTIONTOPDOWN_API APlayerPawn : public ABasePawn
{
	GENERATED_BODY()
	
public:
	APlayerPawn();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;


	UPROPERTY(EditAnywhere, Category = "Movement");
	float MoveSpeed{ 400.f };
	UPROPERTY(EditAnywhere, Category = "Movement");
	float MouseSens{ 2.f };

	void SetForwarInput(float Value);
	float ForwardInput{ 0.f };
	void SetRightInput(float Value);
	float RightInput{ 0.f };
	void SetMouseX(float Value);
	float MouseX{ 0.f };

	
	void Move(float DeltaTime);
	void Rotate();
	
};

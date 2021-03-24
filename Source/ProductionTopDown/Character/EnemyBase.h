// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class APlayerCharacter;
class UScentComponent;

UCLASS()
class PRODUCTIONTOPDOWN_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	virtual void TriggerDeath() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float Reach{500.f};

protected:
	virtual void BeginPlay() override;
	
	void Move(float ScaleSpeed, FVector MoveDir);
	void FollowPlayer();
	bool Attack();
	FVector GetMoveDirFromScent();
	FVector GetMoveOffsetFromWall(float InReach, ECollisionChannel CollisionChannel);
	FVector CalcVector(FVector Target);

	APlayerCharacter* Player{nullptr};
	UScentComponent* ScentComponent{nullptr};
	
	FHitResult GetFirstHitInReach(ECollisionChannel CollisionChannel, FVector LineTraceEnd, bool DrawTraceLine) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    float MoveSpeed{50.f};
	
private:
	
};

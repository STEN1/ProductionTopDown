// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

void AEnemyBase::TriggerDeath()
{
	Super::TriggerDeath();
	Destroy();
	
}

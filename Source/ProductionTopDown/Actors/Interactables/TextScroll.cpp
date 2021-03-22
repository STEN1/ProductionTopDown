// Fill out your copyright notice in the Description page of Project Settings.


#include "TextScroll.h"

ATextScroll::ATextScroll()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}


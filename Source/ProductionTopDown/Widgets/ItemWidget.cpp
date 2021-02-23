// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"

#include "Components/TextBlock.h"
#include "Actor.h"


void UItemWidget::NativeConstruct()
{
	
	if (ItemText)
	{
		ItemText->SetText(FText::FromString("TESTING"));
	}
}

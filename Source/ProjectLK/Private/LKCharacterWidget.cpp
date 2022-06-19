// Fill out your copyright notice in the Description page of Project Settings.


#include "LKCharacterWidget.h"
#include "Components/TextBlock.h"
#include "LKCharacter.h"


void ULKCharacterWidget::SetCharacterName(FString NewName)
{
	//1. NativeContruct() 호출전에 SetCharacterName 함수가 호출되는 경우 TB_CharacterName 세팅이 되어야 한다.
	SetCharacterNameWidget();

	//2.
	if (nullptr != TB_CharacterName)
	{
		TB_CharacterName->SetText(FText::FromString(NewName));
	}

}

void ULKCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetCharacterNameWidget();

}

void ULKCharacterWidget::SetCharacterNameWidget()
{
	if (nullptr == TB_CharacterName)
	{
		TB_CharacterName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_PlayerName")));
	}
}

bool ULKCharacterWidget::IsSetCharacterNameWidget() { 
	return (nullptr != TB_CharacterName); 
}
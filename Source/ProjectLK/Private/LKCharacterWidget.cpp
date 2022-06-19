// Fill out your copyright notice in the Description page of Project Settings.


#include "LKCharacterWidget.h"
#include "Components/TextBlock.h"
#include "LKCharacter.h"


void ULKCharacterWidget::SetCharacterName(FString NewName)
{
	//1. NativeContruct() ȣ������ SetCharacterName �Լ��� ȣ��Ǵ� ��� TB_CharacterName ������ �Ǿ�� �Ѵ�.
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
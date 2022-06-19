// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LKCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLK_API ULKCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCharacterName(FString NewName);
	bool IsSetCharacterNameWidget();

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	class UTextBlock* TB_CharacterName;

	void SetCharacterNameWidget();
};

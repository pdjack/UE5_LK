// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LKCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=ProjectLK)
class PROJECTLKSETTING_API ULKCharacterSetting : public UObject
{
	GENERATED_BODY()
public:
	ULKCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets_Male;

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets_Female;

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets_Female_SchoolGirl;

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets_Male_Citizen;

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets_Female_Citizen;

	UPROPERTY(config)
	TArray<FSoftObjectPath> WeaponAssets;


	UPROPERTY(config)
	TArray<FSoftObjectPath> PetAssets;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsHair;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsTop;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsPants;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsShoes;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsOnepiece;
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsHair_Male;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsTop_Male;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsPants_Male;

	UPROPERTY(config)
	TArray<FSoftObjectPath> PartsShoes_Male;
};

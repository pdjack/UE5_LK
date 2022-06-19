// Fill out your copyright notice in the Description page of Project Settings.


#include "LKGameInstance.h"

ULKGameInstance::ULKGameInstance()
{
	FString ExpDataPath = TEXT("/Game/GameData/Character_-_Exp.Character_-_Exp");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LKEXP(*ExpDataPath);
	LKCHECK(DT_LKEXP.Succeeded());
	LKExpTable = DT_LKEXP.Object;
	LKCHECK(LKExpTable->GetRowMap().Num() > 0);
	
	FString CharacterDataPath = TEXT("/Game/GameData/Character_-_Character.Character_-_Character");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LKCHARACTER(*CharacterDataPath);
	LKCHECK(DT_LKCHARACTER.Succeeded());
	LKCharacterTable = DT_LKCHARACTER.Object;
	LKCHECK(LKCharacterTable->GetRowMap().Num() > 0);

	//DataTable'/Game/GameData/Character_-_Stat.Character_-_Stat'
	FString StatDataPath = TEXT("/Game/GameData/Character_-_Stat.Character_-_Stat");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LKSTAT(*StatDataPath);
	LKCHECK(DT_LKSTAT.Succeeded());
	LKStatTable = DT_LKSTAT.Object;
	LKCHECK(LKStatTable->GetRowMap().Num() > 0);


	
}

void ULKGameInstance::Init()
{
	Super::Init();

	LKLOG(Warning, TEXT("NeedExp of Level 20 : %d"), GetLKExpData(20)->NeedExp);
	LKLOG(Warning, TEXT("Con of class 1 : %d"), GetLKCharacterData(1)->Con);
	
	
	
}

FLKExpData* ULKGameInstance::GetLKExpData(int32 Level)
{
	return LKExpTable->FindRow<FLKExpData>(*FString::FromInt(Level), TEXT(""));
}

FLKCharacterData * ULKGameInstance::GetLKCharacterData(int32 NewClass)
{
	int32 count = LKCharacterTable->GetRowMap().Num();
	for (int i = 1; i <= count; ++i)
	{
		FLKCharacterData* data = LKCharacterTable->FindRow<FLKCharacterData>(*FString::FromInt(i), TEXT(""));
		if (data && data->Class == NewClass)
		{
			return data;
		}

	}

	return nullptr;
	//return LKCharacterTable->FindRow<FLKCharacterData>(*FString::FromInt(RowNum), TEXT(""));
}

FLKStatData * ULKGameInstance::GetLKStatData(int32 RowNum)
{
	//FString MaxHP = FString::Printf(TEXT("MaxHP"));
	//FLKStatData* data = LKStatTable->FindRow<FLKStatData>(*MaxHP, TEXT(""));
	
	FLKStatData* data = LKStatTable->FindRow<FLKStatData>(*FString::FromInt(RowNum), TEXT(""));
	return data;
}

int32 ULKGameInstance::GetLKStatDataNum()
{	
	return LKStatTable->GetRowMap().Num();
}

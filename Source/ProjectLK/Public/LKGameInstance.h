// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "LKGameInstance.generated.h"





USTRUCT(BlueprintType)
struct FLKStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FLKStatData() : StatType(""), Class(0), Base(0),StatRefer(""), Factor(0){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString StatType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString StatRefer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Factor;

	

};


USTRUCT(BlueprintType)
struct FLKCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FLKCharacterData() : Class(1), Str(0), Dex(0), Int(0), Wis(0), Con(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Str;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Dex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Int;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Wis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Con;

};

USTRUCT(BlueprintType)
struct FLKExpData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FLKExpData() : LV(1), NeedExp(1), AddStat(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 LV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NeedExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AddStat;

};

/**
 * 
 */
UCLASS()
class PROJECTLK_API ULKGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	ULKGameInstance();

	virtual void Init() override;
	FLKExpData* GetLKExpData(int32 Level);
	FLKCharacterData* GetLKCharacterData(int32 RowNum);
	FLKStatData* GetLKStatData(int32 RowNum);
	int32 GetLKStatDataNum();

	FStreamableManager StreamableManager;


	

private:

	

	UPROPERTY()
	class UDataTable* LKExpTable;
	
	UPROPERTY()
	class UDataTable* LKCharacterTable;

	UPROPERTY()
	class UDataTable* LKStatTable;
};

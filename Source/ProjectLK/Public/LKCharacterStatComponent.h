// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectLK.h"
#include "Components/ActorComponent.h"
#include "LKCharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLK_API ULKCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULKCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetNewLevel(int32 NewLevel);

	void SetPlayerStatAndAttributes();


	void ResetAttributes();
	void SetAttribute(struct FLKStatData* data, TMap<FString, int32> &StatReferMap, int32 attributeType);

	enum class EStatType : int32
	{
		Str = 1,
		Dex = 2,
		Int =3,
		Wis = 4,
		Con = 5
	};

	enum class EAttributeType : int32
	{
		MaxHP,
		MaxMP,
		Atk,
		Def,
		SkillDmg,
		CriticalProb,
		CriticalDmg,
		Accuracy,
		Aviod,
		MPDecrease,
		HPRecovery,
		MPRecovery,
		
		MoveSpd,
		AttackSpd,
		HitHPAdd,
		HitMPAdd,
		Cooltime,
		FireDmg,
		FireRes,
		WindDmg,
		WindRes,
		IceDmg,
		IceRes,
		EarthDmg,
		EarthRes,
		NormalDmg,
		NormalDef,
		BossDmg,
		BossDef,
		PlayerDmg,
		PlayerDef,
		AddDmg,
		AddDef,

	};

private:

	TMap<FString, int32> StatKeyMap;
	TMap<FString, int32> AttributeKeyMap;

	struct FLKExpData* CurrentExpData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 ClassID;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	//Character Main Stat
	int32 Str = 0;
	int32 Dex = 0;
	int32 Int = 0;
	int32 Wis = 0;
	int32 Con = 0;

	//Character Attributes
	//UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHP = 0;
	float MaxMP = 0;
	float Atk = 0;
	float Def = 0;
	float SkillDmg = 0;
	float CriticalProb = 0;
	float CriticalDmg = 0;
	float Accuracy = 0;
	float Aviod = 0;
	float MPDecrease = 0;
	float HPRecovery = 0;
	float MPRecovery = 0;
	float MoveSpd = 0;
	float AttackSpd = 0;
	float HitHPAdd = 0;
	float HitMPAdd = 0;
	float Cooltime = 0;
	float FireDmg = 0;
	float FireRes = 0;
	float WindDmg = 0;
	float WindRes = 0;
	float IceDmg = 0;
	float IceRes = 0;
	float EarthDmg = 0;
	float EarthRes = 0;
	float NormalDmg = 0;
	float NormalDef = 0;
	float BossDmg = 0;
	float BossDef = 0;
	float PlayerDmg = 0;
	float PlayerDef = 0;
	float AddDmg = 0;
	float AddDef = 0;
		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "LKCharacterStatComponent.h"
#include "LKGameInstance.h"

// Sets default values for this component's properties
ULKCharacterStatComponent::ULKCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
	ClassID = 1; //1=Knight, 4=Priest, 5=Archer
	ResetAttributes();

	StatKeyMap.Add(TEXT("Str"), (int32)EStatType::Str);
	StatKeyMap.Add(TEXT("Dex"), (int32)EStatType::Dex);
	StatKeyMap.Add(TEXT("Int"), (int32)EStatType::Int);
	StatKeyMap.Add(TEXT("Wis"), (int32)EStatType::Wis);
	StatKeyMap.Add(TEXT("Con"), (int32)EStatType::Con);

	AttributeKeyMap.Add(TEXT("MaxHP"), (int32)EAttributeType::MaxHP);
	AttributeKeyMap.Add(TEXT("MaxMP"), (int32)EAttributeType::MaxMP);
	AttributeKeyMap.Add(TEXT("Atk"), (int32)EAttributeType::Atk);
	AttributeKeyMap.Add(TEXT("Def"), (int32)EAttributeType::Def);
	AttributeKeyMap.Add(TEXT("SkillDmg"), (int32)EAttributeType::SkillDmg);
	AttributeKeyMap.Add(TEXT("CriticalProb"), (int32)EAttributeType::CriticalProb);
	AttributeKeyMap.Add(TEXT("CriticalDmg"), (int32)EAttributeType::CriticalDmg);
	AttributeKeyMap.Add(TEXT("Accuracy"), (int32)EAttributeType::Accuracy);
	AttributeKeyMap.Add(TEXT("Aviod"), (int32)EAttributeType::Aviod);
	AttributeKeyMap.Add(TEXT("MPDecrease"), (int32)EAttributeType::MPDecrease);
	AttributeKeyMap.Add(TEXT("HPRecovery"), (int32)EAttributeType::HPRecovery);
	AttributeKeyMap.Add(TEXT("MPRecovery"), (int32)EAttributeType::MPRecovery);

	AttributeKeyMap.Add(TEXT("MoveSpd"), (int32)EAttributeType::MoveSpd);
	AttributeKeyMap.Add(TEXT("AttackSpd"), (int32)EAttributeType::AttackSpd);
	AttributeKeyMap.Add(TEXT("HitHPAdd"), (int32)EAttributeType::HitHPAdd);
	AttributeKeyMap.Add(TEXT("HitMPAdd"), (int32)EAttributeType::HitMPAdd);
	AttributeKeyMap.Add(TEXT("Cooltime"), (int32)EAttributeType::Cooltime);
	AttributeKeyMap.Add(TEXT("FireDmg"), (int32)EAttributeType::FireDmg); 
	AttributeKeyMap.Add(TEXT("FireRes"), (int32)EAttributeType::FireRes);
	AttributeKeyMap.Add(TEXT("WindDmg"), (int32)EAttributeType::WindDmg);
	AttributeKeyMap.Add(TEXT("WindRes"), (int32)EAttributeType::WindRes);
	AttributeKeyMap.Add(TEXT("IceDmg"), (int32)EAttributeType::IceDmg);
	AttributeKeyMap.Add(TEXT("IceRes"), (int32)EAttributeType::IceRes);
	AttributeKeyMap.Add(TEXT("EarthDmg"), (int32)EAttributeType::EarthDmg);
	AttributeKeyMap.Add(TEXT("EarthRes"), (int32)EAttributeType::EarthRes);
	AttributeKeyMap.Add(TEXT("NormalDmg"), (int32)EAttributeType::NormalDmg);
	AttributeKeyMap.Add(TEXT("NormalDef"), (int32)EAttributeType::NormalDef);
	AttributeKeyMap.Add(TEXT("BossDmg"), (int32)EAttributeType::BossDmg);
	AttributeKeyMap.Add(TEXT("BossDef"), (int32)EAttributeType::BossDef);
	AttributeKeyMap.Add(TEXT("PlayerDmg"), (int32)EAttributeType::PlayerDmg);
	AttributeKeyMap.Add(TEXT("PlayerDef"), (int32)EAttributeType::PlayerDef);
	AttributeKeyMap.Add(TEXT("AddDmg"), (int32)EAttributeType::AddDmg);
	AttributeKeyMap.Add(TEXT("AddDef"), (int32)EAttributeType::AddDef);


	
}


// Called when the game starts
void ULKCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();





	//1.
	SetPlayerStatAndAttributes();
	//2.
	CurrentHP = MaxHP;

}

void ULKCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);

}


void ULKCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto LKGameInstance = Cast<ULKGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	LKCHECK(nullptr != LKGameInstance);
	CurrentExpData = LKGameInstance->GetLKExpData(NewLevel);
	if (nullptr != CurrentExpData)
	{
		
		Level = NewLevel;
		LKLOG(Warning, TEXT("Need Exp for NextLevel : %d"), CurrentExpData->NeedExp);
	}
}

void ULKCharacterStatComponent::SetPlayerStatAndAttributes()
{
	auto LKGameInstance = Cast<ULKGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FLKCharacterData * CharacterData = LKGameInstance->GetLKCharacterData(ClassID);
	
	if (nullptr == CharacterData) return;


	//-----------------------------------------------
	// 0. Reset Attributes
	//------------------------------------------------
	ResetAttributes();

	//------------------------------------------------
	// 1. Default Character Stat
	//------------------------------------------------
	Str = CharacterData->Str;
	Dex = CharacterData->Dex;
	Int = CharacterData->Int;
	Wis = CharacterData->Wis;
	Con = CharacterData->Con;

	//-----------------------------------------------
	// 2. added stat ( DB / PassiveSkill )
	//-----------------------------------------------
	//@todo:
	//Str += AddStatStr;
	//Dex += AddStatDex;
	// ...

	//-----------------------------------------------
	// 3. 반드시 Stat 값이 모두 계산된 이후 호출 할것
	//-----------------------------------------------
	TMap<FString, int32> StatReferMap;
	StatReferMap.Add(TEXT("Str"), Str);
	StatReferMap.Add(TEXT("Dex"), Dex);
	StatReferMap.Add(TEXT("Int"), Int);
	StatReferMap.Add(TEXT("Wis"), Wis);
	StatReferMap.Add(TEXT("Con"), Con);


	//-------------------------------
	// 4. StatReferMap 세팅 이후 처리할것
	//--------------------------------
	int32 count = LKGameInstance->GetLKStatDataNum();
	for (int i = 1; i <= count; ++i)
	{
		FLKStatData* data = LKGameInstance->GetLKStatData(i);
		if (data)
			LKLOG(Warning, TEXT("%s"), *data->StatType);

		if (AttributeKeyMap.Contains(*data->StatType))
		{
			int32 attributeType = AttributeKeyMap[*data->StatType];
			int32 statRef = StatKeyMap[*data->StatRefer];

			SetAttribute(data, StatReferMap, attributeType);
		}
	}
}

void ULKCharacterStatComponent::ResetAttributes()
{
	MaxHP = 0;
	MaxMP = 0;
	Atk = 0;
	Def = 0;
	SkillDmg = 0;
	CriticalProb = 0;
	CriticalDmg = 0;
	Accuracy = 0;
	Aviod = 0;
	MPDecrease = 0;
	HPRecovery = 0;
	MPRecovery = 0;
	MoveSpd = 0;
	AttackSpd = 0;
	HitHPAdd = 0;
	HitMPAdd = 0;
	Cooltime = 0;
	FireDmg = 0;
	FireRes = 0;
	WindDmg = 0;
	WindRes = 0;
	IceDmg = 0;
	IceRes = 0;
	EarthDmg = 0;
	EarthRes = 0;
	NormalDmg = 0;
	NormalDef = 0;
	BossDmg = 0;
	BossDef = 0;
	PlayerDmg = 0;
	PlayerDef = 0;
	AddDmg = 0;
	AddDef = 0;
}

void ULKCharacterStatComponent::SetAttribute(FLKStatData* data, TMap<FString, int32> &StatReferMap, int32 attributeType)
{
	switch (attributeType)
	{
	case (int32)EAttributeType::MaxHP:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		MaxHP += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("MaxHP : %f"), MaxHP);
	}
	break;
	case (int32)EAttributeType::MaxMP:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		MaxMP += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("MaxMP : %f"), MaxMP);
	}
	break;
	case (int32)EAttributeType::Atk:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		Atk += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("Atk : %f"), Atk);
	}
	break;
	case (int32)EAttributeType::Def:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		Def += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("Def : %f"), Def);
	}
	break;

	case (int32)EAttributeType::SkillDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		SkillDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("SkillDmg : %f"), SkillDmg);
	}
	break;

	case (int32)EAttributeType::CriticalProb:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		CriticalProb += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("CriticalProb : %f"), CriticalProb);
	}
	break;

	case (int32)EAttributeType::CriticalDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		CriticalDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("CriticalDmg : %f"), CriticalDmg);
	}
	break;

	case (int32)EAttributeType::Accuracy:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		Accuracy += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("Accuracy : %f"), Accuracy);
	}
	break;

	case (int32)EAttributeType::Aviod:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		Aviod += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("Aviod : %f"), Aviod);
	}
	break;
	case (int32)EAttributeType::MPDecrease:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		MPDecrease += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("MPDecrease : %f"), MPDecrease);
	}
	break;
	case (int32)EAttributeType::HPRecovery:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		HPRecovery += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("HPRecovery : %f"), HPRecovery);
	}
	break; 
	case (int32)EAttributeType::MPRecovery:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		MPRecovery += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("MPRecovery : %f"), MPRecovery);
	}
	break;
	case (int32)EAttributeType::MoveSpd:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		MoveSpd += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("MoveSpd : %f"), MoveSpd);
	}
	break;
	case (int32)EAttributeType::AttackSpd:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		AttackSpd += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("AttackSpd : %f"), AttackSpd);
	}
	break;
	case (int32)EAttributeType::HitHPAdd:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		HitHPAdd += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("HitHPAdd : %f"), HitHPAdd);
	}
	break;
	case (int32)EAttributeType::HitMPAdd:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		HitMPAdd += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("HitMPAdd : %f"), HitMPAdd);
	}
	break;
	case (int32)EAttributeType::Cooltime:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		Cooltime += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("Cooltime : %f"), Cooltime);
	}
	break;
	case (int32)EAttributeType::FireDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		FireDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("FireDmg : %f"), FireDmg);
	}
	break;
	case (int32)EAttributeType::FireRes:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		FireRes += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("FireRes : %f"), FireRes);
	}
	break;
	case (int32)EAttributeType::WindDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		WindDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("WindDmg : %f"), WindDmg);
	}
	break;
	case (int32)EAttributeType::WindRes:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		WindRes += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("WindDmg : %f"), WindRes);
	}
	break;
	case (int32)EAttributeType::IceDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		IceDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("IceDmg : %f"), IceDmg);
	}
	break;
	case (int32)EAttributeType::IceRes:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		IceRes += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("IceRes : %f"), IceRes);
	}
	break;
	case (int32)EAttributeType::EarthDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		EarthDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("EarthDmg : %f"), EarthDmg);
	}
	break;
	case (int32)EAttributeType::EarthRes:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		EarthRes += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("EarthRes : %f"), EarthRes);
	}
	break;
	case (int32)EAttributeType::NormalDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		NormalDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("NormalDmg : %f"), NormalDmg);
	}
	break;
	case (int32)EAttributeType::NormalDef:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		NormalDef += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("NormalDef : %f"), NormalDef);
	}
	break;
	case (int32)EAttributeType::BossDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		BossDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("BossDmg : %f"), BossDmg);
	}
	break;
	case (int32)EAttributeType::BossDef:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		BossDef += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("BossDef : %f"), BossDef);
	}
	break;
	case (int32)EAttributeType::PlayerDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		PlayerDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("PlayerDmg : %f"), PlayerDmg);
	}
	break;
	case (int32)EAttributeType::PlayerDef:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		PlayerDef += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("PlayerDef : %f"), PlayerDef);
	}
	break;
	case (int32)EAttributeType::AddDmg:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		AddDmg += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("PlayerDef : %f"), AddDmg);
	}
	break;
	case (int32)EAttributeType::AddDef:
	{
		int32 statReferValue = StatReferMap[*data->StatRefer];
		AddDef += (data->Base + ((float)statReferValue * data->Factor));
		LKLOG(Warning, TEXT("AddDef : %f"), AddDef);
	}
	break;
	}
	
}

// Called every frame
//void ULKCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}


// Fill out your copyright notice in the Description page of Project Settings.


#include "LKWeapon.h"
#include "LKCharacterSetting.h"
#include "LKGameInstance.h"
// Sets default values
ALKWeapon::ALKWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void ALKWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALKWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALKWeapon::SetMesh(int32 WeaponNum)
{
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	WeaponAssetToLoad = DefaultSetting->WeaponAssets[WeaponNum];

	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{
		AssetStreamingHandle = LKGameInstance->StreamableManager.RequestAsyncLoad(WeaponAssetToLoad, FStreamableDelegate::CreateUObject(this, &ALKWeapon::OnAssetLoadCompleted));
	}
}

void ALKWeapon::OnAssetLoadCompleted()
{
	LKLOG(Warning, TEXT("ALKWeapon::OnAssetLoadCompleted()"));

	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(WeaponAssetToLoad);

	if (LoadedAssetPath.IsValid())
	{
		Weapon->SetSkeletalMesh(LoadedAssetPath.Get());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LKPet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "LKCharacterSetting.h"
#include "LKGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "LKCharacter.h"
// Sets default values
ALKPet::ALKPet()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	static ConstructorHelpers::FClassFinder<UAnimInstance> PET_ANIM(TEXT("/Game/Cute_Whale/Animations/BP_Whale_Anim.BP_Whale_Anim_C"));
	if (PET_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PET_ANIM.Class);		
	}


	
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetEnableGravity(false);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetEnableGravity(false);


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PetAssetNum = -1;

}

// Called when the game starts or when spawned
void ALKPet::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ALKPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != DestTarget)
	{
		FVector Location = GetActorLocation();
		FVector DestLocation = DestTarget->GetActorLocation() + (DestTarget->GetActorRightVector() * 50.0f);
		float const Distance = FVector::Dist(Location, DestLocation);
		if (Distance < 100.0f)
		{
			//LKLOG(Warning, TEXT("Distance < 100.0f"));
		}

		
		float const DistanceToTarget = FVector::Dist(Location, DestLocation);
		if (DistanceToTarget >= 100.0f)
		{
			//SetDestination(DestTarget->GetActorLocation());
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

			//DestLocation = NewDestination;
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), DestLocation);
			
		}
		
	}

	

}

//서버에서만 호출되야 한다.(서버에 스폰된 Pet)
void ALKPet::SetAssetNum(int32 NewAsset)
{
	PetAssetNum = NewAsset;

	//서버에서만 호출될때 host를 위해
	OnRep_PetAsset();
}

void ALKPet::SetMeshScale3D(float size)
{
	if (GetMesh())
	{
		GetMesh()->SetWorldScale3D(FVector(size, size, size));
	}
}


void ALKPet::SetDestTarget(ACharacter * NewDestTarget)
{
	DestTarget = NewDestTarget;
}

void ALKPet::SetMesh(int32 PetNum)
{
	//잠깐 주석처리
	auto DefaultSetting = GetDefault<ULKCharacterSetting>();
	PetAssetToLoad = DefaultSetting->PetAssets[PetNum];

	auto LKGameInstance = Cast<ULKGameInstance>(GetGameInstance());
	if (nullptr != LKGameInstance)
	{
		AssetStreamingHandle = LKGameInstance->StreamableManager.RequestAsyncLoad(PetAssetToLoad, FStreamableDelegate::CreateUObject(this, &ALKPet::OnAssetLoadCompleted));
	}
}

void ALKPet::OnAssetLoadCompleted()
{
	LKLOG(Warning, TEXT("ALKPet::OnAssetLoadCompleted()"));

	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(PetAssetToLoad);

	if (LoadedAssetPath.IsValid())
	{
		
		GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
	}
}


void ALKPet::OnRep_PetAsset()
{
	SetMesh(PetAssetNum);
}



void ALKPet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALKPet, OwnerPlayerId)
	DOREPLIFETIME(ALKPet, OwnerPCName)
	DOREPLIFETIME(ALKPet, PetAssetNum)


}